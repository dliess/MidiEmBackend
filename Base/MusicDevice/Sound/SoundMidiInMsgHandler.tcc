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

   m_pMidiInIf->registerMidiInCb(
       [this](const midi::MidiMessage& midiMsg) {
          const auto midiId = midiMessageToId(midiMsg);
          if (mpark::holds_alternative<mpark::monostate>(midiId))
          {
             return;
          }
          auto iter = m_map.find(midiId);
          if (m_map.end() == iter)
          {
             LOG_F(ERROR, "SOUND --- {} No mapping for Midi msg id {}",
                   m_pMidiInIf->medium().getDeviceName(),
                   meta::serialize(midiId).dump());
             return;
          }
          handleSoundDevParameterRouting(midiMsg, iter->second);
       });
}

template <typename MidiInIfPtr>
int sound::MidiInMsgHandler<MidiInIfPtr>::midiChannelNr2VoiceId(
    int midiChannel, int engineId) const noexcept
{
   if (engineId == base::musicDevice::description::sound::GlobalSectionId)
   {
      return base::musicDevice::description::sound::GlobalSectionId;
   }
   for (int i = 0; i < m_rSoundSection.voices.size(); ++i)
   {
      const auto& voice = m_rSoundSection.voices[i];
      if (voice.midiChannel == (midiChannel - m_midiVoiceOffset) &&
          voice.engineId == engineId)
      {
         return i;
      }
   }
   assert(false);
   return -2;
}

template <typename MidiInIfPtr>
void sound::MidiInMsgHandler<MidiInIfPtr>::handleSoundDevParameterRouting(
    const midi::MidiMessage& midiMsg,
    const base::musicDevice::description::sound::ParameterId& id) const noexcept
{
   mpark::visit(
       midi::overload{
           [this, &id](const midi::Message<midi::ControlChange>& msg) {
              m_drainCb(midiChannelNr2VoiceId(msg.channel(), id.engineId),
                        id.parameterId, msg.getRelativeValue());
           },
           [this, &id](const midi::Message<midi::ControlChangeHighRes>& msg) {
              m_drainCb(midiChannelNr2VoiceId(msg.channel(), id.engineId),
                        id.parameterId, msg.getRelativeValue());
           },
           [this, &id](const midi::Message<midi::NRPN>& msg) {
              m_drainCb(midiChannelNr2VoiceId(msg.channel(), id.engineId),
                        id.parameterId, msg.getRelativeValue());
           },
           [this, &id](const midi::Message<midi::RPN>& msg) {
              m_drainCb(midiChannelNr2VoiceId(msg.channel(), id.engineId),
                        id.parameterId, msg.getRelativeValue());
           },
           [](auto&& other) {}},
       midiMsg);
}

template <typename MidiInIfPtr>
void sound::MidiInMsgHandler<MidiInIfPtr>::initCacheBySoundSection() noexcept
{
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
          m_map[parameter.source.midi->id] = paramId;
       });
}

}   // namespace base::musicDevice