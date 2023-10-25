#include "MidiRouter.h"
#include "MidiMessage.h"

using namespace base::midifriends;

Router::Router(musicDevice::MidiHolder& rMidiHolder) noexcept :
    m_rMidiHolder(rMidiHolder)
{
   m_rMidiHolder.registerForInputAdded(
       [this](const std::shared_ptr<musicDevice::MusicDevice::MidiInput>&
                  pMidiIn) {
          const musicDevice::MidiHolder::Id id(
              pMidiIn->medium().getDevicePortName(),
              pMidiIn->medium().getHostConnectorPortName());
          pMidiIn->registerMidiInCb(
              [this, id](const midi::MidiMessage& midiMsg) {
                 handleMidiIn(id, midiMsg);
              });
       });

   m_rMidiHolder.registerForInputRemoved(
       [this](const musicDevice::MidiHolder::Id& id) {
          m_routingData.erase(id);
       });

   m_rMidiHolder.registerForOutputRemoved(
       [this](const musicDevice::MidiHolder::Id& id) {
          for (auto& e : m_routingData) { e.second.erase(id); }
       });
}

void Router::handleMidiIn(const musicDevice::MidiHolder::Id& id,
                          const midi::MidiMessage& midiMsg) noexcept
{
   const auto itIn = m_routingData.find(id);
   if (itIn == m_routingData.end())
   {
      return;
   }
   for (auto& [outId, routingData] : itIn->second)
   {
      assert(routingData.pMidiOut);
      if (!routingData.routed)
      {
         continue;
      }    
      
      if (routingData.specialized)
      {
         handleSpecialized(midiMsg, routingData);
      }
      else
      {
         routingData.pMidiOut->send(midiMsg);
         SWITCH(midiMsg)
            CASE(midi::Message<midi::NoteOn>, note) {
              routingData.noteOnMap.setNoteOn(note.channel() - 1, note.noteNumber());
            },
            CASE(midi::Message<midi::NoteOff>, note) {
              routingData.noteOnMap.setNoteOff(note.channel() - 1, note.noteNumber());
            },
            CASE_DEFAULT {}
         END_SWITCH
      }
   }
}

void Router::handleSpecialized(
    const midi::MidiMessage& midiMsg, RoutingData& routingData) noexcept
{
   dl::visit(
       dl::overload{
           [&routingData](const midi::Message<midi::Clock>& msg) {
              if (routingData.specialized->transmitClockMsg)
              {
                 routingData.pMidiOut->send(msg);
              }
           },
           [this, &routingData](const midi::Message<midi::NoteOn>& msg) {
              const auto hasSentOnChannel = handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
              if (hasSentOnChannel)
              {
                 routingData.noteOnMap.setNoteOn(*hasSentOnChannel, msg.noteNumber());
              }
           },
           [this, &routingData](const midi::Message<midi::NoteOff>& msg) {
               const auto hasSentOnChannel = handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
               if (hasSentOnChannel)
               {
                  routingData.noteOnMap.setNoteOff(*hasSentOnChannel, msg.noteNumber());
               }
           },
           [this, &routingData](const midi::Message<midi::AfterTouchPoly>& msg) {
              handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
           },
           [this, &routingData](const midi::Message<midi::ControlChange>& msg) {
              handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
           },
           [this, &routingData](const midi::Message<midi::ProgramChange>& msg) {
              handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
           },
           [this, &routingData](const midi::Message<midi::AfterTouchChannel>& msg) {
              handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
           },
           [this, &routingData](const midi::Message<midi::PitchBend>& msg) {
              handleVoiceMsg(routingData.specialized->channelMapping, msg, *routingData.pMidiOut);
           },
           [](auto&& other) {}},
       midiMsg);
}

bool Router::isRoutedTo(const musicDevice::MidiHolder::Id& source,
                        const musicDevice::MidiHolder::Id& dest) const noexcept
{
   const auto routingData = getRoutingData(source, dest);
   return (nullptr != routingData && routingData->routed);
}

void Router::toggleRouted(const musicDevice::MidiHolder::Id& source,
                          const musicDevice::MidiHolder::Id& dest) noexcept
{
   auto itSrc = m_routingData.find(source);
   if (itSrc == m_routingData.end())
   {
      auto pMidiOut = m_rMidiHolder.getMidiOut(dest);
      assert(pMidiOut);
      m_routingData.emplace(std::make_pair(
          source,
          DstType({{dest, {true, std::move(pMidiOut), std::nullopt}}})));
   }
   else
   {
      const auto itDst = itSrc->second.find(dest);
      if (itDst == itSrc->second.end())
      {
         auto pMidiOut = m_rMidiHolder.getMidiOut(dest);
         assert(pMidiOut);
         itSrc->second.emplace(std::make_pair(
             dest, RoutingData{true, std::move(pMidiOut), std::nullopt}));
      }
      else
      {
         itDst->second.routed = !itDst->second.routed;
         if (!itDst->second.routed)
         {
            itDst->second.noteOnMap.forEachNoteOn(
                [this, &itDst](int channel, int note) {
                   itDst->second.pMidiOut->send(
                       midi::Message<midi::NoteOff>(channel + 1, note, 0));
                });
            itDst->second.noteOnMap.clear();
         }
      }
   }
   for (auto& cb : m_routedChangedCBs)
      cb(source, dest, isRoutedTo(source, dest));
}

bool Router::hasSpecializedData(
    const musicDevice::MidiHolder::Id& source,
    const musicDevice::MidiHolder::Id& dest) const noexcept
{
   const auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      return false;
   }
   return pRoutingData->specialized.has_value();
}

void Router::toggleSpecializedRoutingEnabled(
    const musicDevice::MidiHolder::Id& source,
    const musicDevice::MidiHolder::Id& dest) noexcept
{
   if(hasSpecializedData(source, dest))
   {
      clearSpecialized(source, dest);
   }
   else
   {
      initSpecialized(source, dest);
   }
}

void Router::initSpecialized(const musicDevice::MidiHolder::Id& source,
                             const musicDevice::MidiHolder::Id& dest) noexcept
{
   auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      toggleRouted(source, dest);
      toggleRouted(source, dest);
      pRoutingData = getRoutingData(source, dest);
   }
   if (pRoutingData->specialized)
      return;
   pRoutingData->specialized.emplace(RoutingDataSpecialized());
   for (auto& cb : m_specialRoutedChangedCBs) cb(source, dest, true);
}

void Router::clearSpecialized(const musicDevice::MidiHolder::Id& source,
                              const musicDevice::MidiHolder::Id& dest) noexcept
{
   auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      return;
   }
   pRoutingData->specialized.reset();
   for (auto& cb : m_specialRoutedChangedCBs) cb(source, dest, false);
}

uint16_t Router::getMappingFor(const musicDevice::MidiHolder::Id& source,
                               const musicDevice::MidiHolder::Id& dest,
                               int sourceChannelIdx) const noexcept
{
   const auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      return 0;
   }
   if (sourceChannelIdx < 0 || sourceChannelIdx > 15)
   {
      return 0;
   }
   if (!pRoutingData->specialized)
   {
      return (1 << sourceChannelIdx);
   }
   return pRoutingData->specialized->channelMapping[sourceChannelIdx];
}

void Router::toggleMappingForChannelIdx(
    const musicDevice::MidiHolder::Id& source,
    const musicDevice::MidiHolder::Id& dest, int sourceChannelIdx,
    int destinationChannelIdx) noexcept
{
   const auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      return;
   }
   if (sourceChannelIdx < 0 || sourceChannelIdx > 15)
   {
      return;
   }
   if (destinationChannelIdx < 0 || destinationChannelIdx > 15)
   {
      return;
   }
   if (!pRoutingData->specialized)
   {
      return;
   }
   const bool enabled =
       pRoutingData->specialized->channelMapping[sourceChannelIdx] &
       (1 << destinationChannelIdx);
   if (enabled)
   {
      pRoutingData->specialized->channelMapping[sourceChannelIdx] &=
          ~(1 << destinationChannelIdx);
      pRoutingData->noteOnMap.forEachNoteOn(
          [this, &pRoutingData, sourceChannelIdx, destinationChannelIdx](
              int channel, int note) {
             if (channel == sourceChannelIdx)
             {
                pRoutingData->pMidiOut->send(midi::Message<midi::NoteOff>(
                    destinationChannelIdx + 1, note, 0));
             }
          });
      pRoutingData->noteOnMap.clear();
   }
   else
   {
      pRoutingData->specialized->channelMapping[sourceChannelIdx] |=
          (1 << destinationChannelIdx);
   }
   for (auto& cb : m_specialRouteChangedCBs)
      cb(source, dest, sourceChannelIdx, destinationChannelIdx, !enabled);
}

void Router::registerRoutedChangedCB(RoutedChangedCB cb)
{
   m_routedChangedCBs.push_back(cb);
}

void Router::registerSpecialRoutedChangedCB(SpecialRoutedChangedCB cb)
{
   m_specialRoutedChangedCBs.push_back(cb);
}

void Router::registerSpecialRouteChangedCB(SpecialRouteChangedCB cb)
{
   m_specialRouteChangedCBs.push_back(cb);
}

void Router::retriggerCallbacks()
{
   for (auto& [inId, dest] : m_routingData)
   {
      for (auto& [outId, routingData] : dest)
      {
         for (auto& cb : m_routedChangedCBs) cb(inId, outId, true);
         if (routingData.specialized)
         {
            for (auto& cb : m_specialRoutedChangedCBs) cb(inId, outId, true);
            for (int i = 0; i < routingData.specialized->channelMapping.size();
                 ++i)
            {
               const auto mapData = routingData.specialized->channelMapping[i];
               for (int j = 0; j < 16; ++j)
               {
                  if (mapData & (1 << j))
                  {
                     for (auto& cb : m_specialRouteChangedCBs)
                        cb(inId, outId, i, j, true);
                  }
               }
            }
         }
      }
   }
}

Router::Settings Router::getSettings() const noexcept { return m_routingData; }

void Router::setSettings(const Settings& settings) noexcept
{
   m_routingData = settings;
}
