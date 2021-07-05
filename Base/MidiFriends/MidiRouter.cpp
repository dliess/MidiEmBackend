#include "MidiRouter.h"

using namespace base::midifriends;

Router::Router(musicDevice::MidiHolder& rMidiHolder) noexcept :
    m_rMidiHolder(rMidiHolder)
{
   m_rMidiHolder.registerForInputAdded(
       [this](const std::shared_ptr<musicDevice::MusicDevice::MidiInput>&
                  pMidiIn) {
          const musicDevice::MidiHolder::Id id(
              pMidiIn->medium().getDeviceName(),
              pMidiIn->medium().getPortName());
          pMidiIn->registerMidiInCb(
              [this, &id](const midi::MidiMessage& midiMsg) {
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
   for (const auto& [outId, routingData] : itIn->second)
   {
      assert(routingData.pMidiOut);
      if (routingData.specialized)
      {
         handleSpecialized(midiMsg, *routingData.specialized,
                           *routingData.pMidiOut);
      }
      else
      {
         routingData.pMidiOut->send(midiMsg);
      }
   }
}

void Router::handleSpecialized(
    const midi::MidiMessage& midiMsg, const RoutingDataSpecialized& specialized,
    musicDevice::MusicDevice::MidiOutput& midiOut) noexcept
{
   mpark::visit(
       midi::overload{
           [&specialized, &midiOut](const midi::Message<midi::Clock>& msg) {
              if (specialized.transmitClockMsg)
              {
                 midiOut.send(msg);
              }
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::NoteOn>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::NoteOff>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::AfterTouchPoly>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::ControlChange>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::ProgramChange>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::AfterTouchChannel>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [this, &specialized,
            &midiOut](const midi::Message<midi::PitchBend>& msg) {
              handleVoiceMsg(specialized.channelMapping, msg, midiOut);
           },
           [](auto&& other) {}},
       midiMsg);
}

bool Router::isRoutedTo(const musicDevice::MidiHolder::Id& source,
                        const musicDevice::MidiHolder::Id& dest) const noexcept
{
   return nullptr != getRoutingData(source, dest);
}

void Router::toggleRouted(const musicDevice::MidiHolder::Id& source,
                          const musicDevice::MidiHolder::Id& dest) noexcept
{
   auto itSrc = m_routingData.find(source);
   if (itSrc == m_routingData.end())
   {
      auto pMidiOut = m_rMidiHolder.getMidiOut(dest);
      m_routingData.emplace(std::make_pair(
          source, DstType({{dest, {std::move(pMidiOut), std::nullopt}}})));
   }
   else
   {
      const auto itDst = itSrc->second.find(source);
      if (itDst == itSrc->second.end())
      {
         auto pMidiOut = m_rMidiHolder.getMidiOut(dest);
         itSrc->second.emplace(
             std::make_pair(dest, RoutingData{pMidiOut, std::nullopt}));
      }
      else
      {
         itSrc->second.erase(dest);
         if (itSrc->second.empty())
         {
            m_routingData.erase(itSrc);
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

void Router::initSpecialized(const musicDevice::MidiHolder::Id& source,
                             const musicDevice::MidiHolder::Id& dest) noexcept
{
   auto pRoutingData = getRoutingData(source, dest);
   if (nullptr == pRoutingData)
   {
      return;
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

void Router::setMappingForChannelIdx(const musicDevice::MidiHolder::Id& source,
                                     const musicDevice::MidiHolder::Id& dest,
                                     int sourceChannelIdx,
                                     int destinationChannelIdx,
                                     bool enable) noexcept
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
   if (enable)
   {
      pRoutingData->specialized->channelMapping[sourceChannelIdx] |=
          (1 << destinationChannelIdx);
   }
   else
   {
      pRoutingData->specialized->channelMapping[sourceChannelIdx] &=
          ~(1 << destinationChannelIdx);
   }
   for (auto& cb : m_specialRouteChangedCBs)
      cb(source, dest, sourceChannelIdx, destinationChannelIdx, enable);
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