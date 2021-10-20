#include <loguru.hpp>
#include <mpark/variant.hpp>

#include "JsonCast.h"
#include "MidiMessageIdsMeta.h"
#include "SoundMidiInMsgHandler.h"

namespace base::musicDevice
{
template <typename MidiInIfPtr>
std::string sound::MidiInMsgHandler<MidiInIfPtr>::cache2Str(const Map& map)
{
   std::string ret;
   for (auto iter = map.begin(); iter != map.end(); iter++)
   {
      ret.append(meta::serialize(iter->first).dump() + " --> " +
                 meta::serialize(iter->second).dump() + "\n");
   }
   return ret;
}

template <typename MidiInIfPtr>
sound::MidiInMsgHandler<MidiInIfPtr>::MidiInMsgHandler(
    MidiInIfPtr pMidiInIf, const description::sound::Section& rSoundSection,
    uint8_t midiVoiceOffset, Cb cb) noexcept :
    m_pMidiInIf(pMidiInIf),
    m_rSoundSection(rSoundSection),
    m_midiVoiceOffset(midiVoiceOffset),
    m_drainCb(cb)
{
   initCacheBySoundSection();
   // LOG_F(INFO, "Initialized Sound cache \n{}", cache2Str(m_map));

   m_pMidiInIf->registerMidiInCb([this](const midi::MidiMessage& midiMsg) {
      /*
      LOG_F(INFO, "SOUND --- {} Received {}",
         m_pMidiInIf->medium().getDeviceName(), midi::toString(midiMsg));
      */
      const std::optional<int> voiceIdx = getVoiceIdFromMidiMsg(midiMsg);
      if(!voiceIdx)
      {
         /*
         LOG_F(INFO, "SOUND --- {} Strange message received {}",
         m_pMidiInIf->medium().getDeviceName(), midi::toString(midiMsg));  
         */
         return;
      }
      const auto& map   = m_maps[m_rSoundSection.voice2EngineIdx(*voiceIdx) + 1];
      const auto midiId = midiMessageToId(midiMsg);
      if (mpark::holds_alternative<mpark::monostate>(midiId))
      {
         return;
      }
      auto iter = map.find(midiId);
      if (map.end() == iter)
      {
        /* 
         LOG_F(INFO, "SOUND --- {} No mapping for Midi msg id {}",
               m_pMidiInIf->medium().getDeviceName(),
               meta::serialize(midiId).dump());
         */
         return;
      }
      const float val = getValueBy(midiMsg, iter->second);
      m_drainCb(*voiceIdx, iter->second.parameterId, val);
   });
}

template <typename MidiInIfPtr>
std::optional<int> sound::MidiInMsgHandler<MidiInIfPtr>::getVoiceIdFromMidiMsg(
    const midi::MidiMessage& midiMsg) const noexcept
{
   int midiChannelNumber = mpark::visit(
       util::overload{
           [](const midi::Message<midi::NoteOff>& msg) -> int {
              return msg.channel();
           },
           [](const midi::Message<midi::NoteOn>& msg) -> int {
              return msg.channel();
           },
           [](const midi::Message<midi::ControlChange>& msg) -> int {
              return msg.channel();
           },
           [](const midi::Message<midi::ControlChangeHighRes>& msg) -> int {
              return msg.channel();
           },
           [](const midi::Message<midi::RPN>& msg) -> int {
              return msg.channel();
           },
           [](const midi::Message<midi::NRPN>& msg) -> int {
              return msg.channel();
           },
           [](auto&& msg) -> int {
              return 0;
           }},
       midiMsg);
   if(0 == midiChannelNumber)
   { 
      return std::nullopt;
   }
   int voiceIdx = midiChannelNumber -1 -m_midiVoiceOffset;
   if (m_rSoundSection.global &&
       m_rSoundSection.global->midiChannel == voiceIdx)
   {
      voiceIdx = description::sound::GlobalSectionId;
   }
   return voiceIdx;
}

template <typename MidiInIfPtr>
float sound::MidiInMsgHandler<MidiInIfPtr>::getValueBy(
    const midi::MidiMessage& midiMsg,
    const description::sound::ParameterId& id) const noexcept
{
   const auto& descr = m_rSoundSection.parameterDescr(id);
   assert(descr.source.midi.has_value());

   return mpark::visit(
       midi::overload{
           [this,
            &descr](const midi::Message<midi::ControlChange>& msg) -> float {
              if (descr.type == description::sound::Parameter::Type::List)
              {
                 return descr.getListIndexByValue(msg.controllerValue());
              }
              else
              {
                 const auto& valueRange = descr.source.midi->sourceValueRange;
                 return valueRange.has_value()
                            ? msg.getRelativeValue(valueRange->from,
                                                   valueRange->to)
                            : msg.getRelativeValue();
              }
           },
           [this, &descr](
               const midi::Message<midi::ControlChangeHighRes>& msg) -> float {
              if (descr.type == description::sound::Parameter::Type::List)
              {
                 return descr.getListIndexByValue(msg.controllerValue());
              }
              else
              {
                 const auto& valueRange = descr.source.midi->sourceValueRange;
                 return valueRange.has_value()
                            ? msg.getRelativeValue(valueRange->from,
                                                   valueRange->to)
                            : msg.getRelativeValue();
              }
           },
           [this, &descr](const midi::Message<midi::NRPN>& msg) -> float {
              if (descr.type == description::sound::Parameter::Type::List)
              {
                 return descr.getListIndexByValue(msg.getValue());
              }
              else
              {
                 const auto& valueRange = descr.source.midi->sourceValueRange;
                 return valueRange.has_value()
                            ? msg.getRelativeValue(valueRange->from,
                                                   valueRange->to)
                            : msg.getRelativeValue();
              }
           },
           [this, &descr](const midi::Message<midi::RPN>& msg) -> float {
              if (descr.type == description::sound::Parameter::Type::List)
              {
                 return descr.getListIndexByValue(msg.getValue());
              }
              else
              {
                 const auto& valueRange = descr.source.midi->sourceValueRange;
                 return valueRange.has_value()
                            ? msg.getRelativeValue(valueRange->from,
                                                   valueRange->to)
                            : msg.getRelativeValue();
              }
           },
           [](auto&& other) -> float { return -1; }},
       midiMsg);
}

template <typename MidiInIfPtr>
void sound::MidiInMsgHandler<MidiInIfPtr>::initCacheBySoundSection() noexcept
{
   m_maps.resize(m_rSoundSection.engines.size() + 1);
   m_rSoundSection.forEachParameterDescr(
       [this](const description::sound::ParameterId& paramId,
              const description::sound::Parameter& parameter) {
          assert(parameter.source.midi);
          mpark::visit(
              midi::overload{
                  [this](const midi::MidiMsgId<midi::ControlChangeHighRes>&
                             msgId) {
                     m_pMidiInIf->setCCHighResPair(msgId.idMsb, msgId.idLsb);
                  },
                  [](auto&& other) {}},
              parameter.source.midi->id);
          if (parameter.component)
          {
             bool found = false;
             m_rSoundSection.forEachComponentDescr(
                 paramId.engineId,
                 [&parameter, &found](
                     int componentIdx,
                     const description::sound::ComponentVar& component) {
                    const std::string compName = mpark::visit(
                        util::overload{
                            [](const description::sound::Component& c)
                                -> std::string { return c.name; },
                            [](const description::sound::OneOfComponents& c)
                                -> std::string { return std::string(); }},
                        component);
                    if (*parameter.component == compName)
                    {
                       found = true;
                    }
                 });
             if (!found)
             {
                return;
             }
          }
          m_maps[paramId.engineId + 1][parameter.source.midi->id] = paramId;
       });
}

template <typename MidiInIfPtr>
void sound::MidiInMsgHandler<MidiInIfPtr>::changeMapping(
    int voiceIdx, const std::string& compNamePrev,
    const std::string& compNameNew) noexcept
{
   const int engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   m_rSoundSection.forEachParameterDescr(
       engineIdx,
       [this, engineIdx, &compNamePrev, &compNameNew](
           int paramIdx, const description::sound::Parameter& parameter) {
          if (!parameter.component)
             return;
          if (parameter.component.value() == compNamePrev)
          {
             m_maps[engineIdx + 1].erase(parameter.source.midi->id);
          }
          if (parameter.component.value() == compNameNew)
          {
             m_maps[engineIdx + 1][parameter.source.midi->id] =
                 description::sound::ParameterId({engineIdx, paramIdx});
          }
       });
}

}   // namespace base::musicDevice