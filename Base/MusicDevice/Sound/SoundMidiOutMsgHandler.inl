#include <spdlog/spdlog.h>

#include "MidiMessage.h"
#include "MidiMessageIds.h"
#include "ParameterDumpRequest.h"
#include "SoundMidiOutMsgHandler.h"

namespace base::musicDevice
{
template <typename MidiOutIfPtr>
sound::MidiOutMsgHandler<MidiOutIfPtr>::MidiOutMsgHandler(
    MidiOutIfPtr pMidiOutIf, const description::sound::Section& rSoundSection,
    uint8_t midiChannelOffset) noexcept :
    m_pMidiOutIf(pMidiOutIf),
    m_rSoundSection(rSoundSection),
    m_midiChannelOffset(midiChannelOffset)
{
   if (m_rSoundSection.pitchBendFactor)
   {
      m_pitchBendFactor = *m_rSoundSection.pitchBendFactor;
   }
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::sendSoundParameter(
    uint32_t voiceId, uint32_t parameterId, float value) noexcept
{
   // spdlog::info( "sendSoundParameter {} {} {}", voiceId, parameterId, value);
   const auto& paramDescr =
       m_rSoundSection.parameterDescr(voiceId, parameterId);
   const auto midiChannel =
       m_rSoundSection.getMidiChannel(voiceId) + m_midiChannelOffset;
   assert(paramDescr.source.midi);
   const auto& valueRange = paramDescr.source.midi->sourceValueRange;

   if (paramDescr.type == description::sound::Parameter::Type::List)
   {
      const int val =
          paramDescr.source.midi->sourceRanges->at(int(value)).range.from;
      const auto midiMsg = mpark::visit(
          midi::overload{
              [midiChannel,
               val](const midi::MidiMsgId<midi::ControlChange>& msgId)
                  -> midi::MidiMessage {
                 return midi::Message<midi::ControlChange>(midiChannel,
                                                           msgId.id, val);
              },
              [midiChannel,
               val](const midi::MidiMsgId<midi::ControlChangeHighRes>& msgId)
                  -> midi::MidiMessage {
                 return midi::Message<midi::ControlChangeHighRes>(
                     midiChannel, msgId.idMsb, msgId.idLsb, val);
              },
              [midiChannel, val](const midi::MidiMsgId<midi::NRPN>& msgId)
                  -> midi::MidiMessage {
                 return midi::Message<midi::NRPN>(midiChannel, msgId.idMsb,
                                                  msgId.idLsb, val);
              },
              [midiChannel, val](const midi::MidiMsgId<midi::RPN>& msgId)
                  -> midi::MidiMessage {
                 return midi::Message<midi::RPN>(midiChannel, msgId.idMsb,
                                                 msgId.idLsb, val);
              },
              [](auto&& other) -> midi::MidiMessage {
                 return midi::MidiMessage();
              }},
          paramDescr.source.midi->id);
      m_pMidiOutIf->send(midiMsg);
   }
   else
   {
      const auto midiMsg = mpark::visit(
          midi::overload{
              [midiChannel, value,
               &valueRange](const midi::MidiMsgId<midi::ControlChange>& msgId)
                  -> midi::MidiMessage {
                 if (valueRange)
                 {
                    return midi::Message<midi::ControlChange>::
                        fromRelativeValue(midiChannel, msgId.id, value,
                                          valueRange->from, valueRange->to);
                 }
                 else
                 {
                    return midi::Message<
                        midi::ControlChange>::fromRelativeValue(midiChannel,
                                                                msgId.id,
                                                                value);
                 }
              },
              [midiChannel, value, &valueRange](
                  const midi::MidiMsgId<midi::ControlChangeHighRes>& msgId)
                  -> midi::MidiMessage {
                 if (valueRange)
                 {
                    return midi::Message<midi::ControlChangeHighRes>::
                        fromRelativeValue(midiChannel, msgId.idMsb, msgId.idLsb,
                                          value, valueRange->from,
                                          valueRange->to);
                 }
                 else
                 {
                    return midi::Message<midi::ControlChangeHighRes>::
                        fromRelativeValue(midiChannel, msgId.idMsb, msgId.idLsb,
                                          value);
                 }
              },
              [midiChannel, value,
               &valueRange](const midi::MidiMsgId<midi::NRPN>& msgId)
                  -> midi::MidiMessage {
                 if (valueRange)
                 {
                    return midi::Message<midi::NRPN>::fromRelativeValue(
                        midiChannel, msgId.idMsb, msgId.idLsb, value,
                        valueRange->from, valueRange->to);
                 }
                 else
                 {
                    return midi::Message<midi::NRPN>::fromRelativeValue(
                        midiChannel, msgId.idMsb, msgId.idLsb, value);
                 }
              },
              [midiChannel, value,
               &valueRange](const midi::MidiMsgId<midi::RPN>& msgId)
                  -> midi::MidiMessage {
                 if (valueRange)
                 {
                    return midi::Message<midi::RPN>::fromRelativeValue(
                        midiChannel, msgId.idMsb, msgId.idLsb, value,
                        valueRange->from, valueRange->to);
                 }
                 else
                 {
                    return midi::Message<midi::RPN>::fromRelativeValue(
                        midiChannel, msgId.idMsb, msgId.idLsb, value);
                 }
              },
              [](auto&& other) -> midi::MidiMessage {
                 return midi::MidiMessage();
              }},
          paramDescr.source.midi->id);
      m_pMidiOutIf->send(midiMsg);
   }
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::sendParameterDumpRequest() noexcept
{
   ParameterDumpRequest(*m_pMidiOutIf, m_rSoundSection, m_midiChannelOffset)
       .sendParameterDumpRequest();
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::noteOn(int voiceIndex, int note,
                                                    float velocity) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   const auto engineIdx   = voiceDescr.engineId;
   const auto& engineDesc = m_rSoundSection.engines[engineIdx];
   if (engineDesc.noteSettings)
   {
      const int noteRangeStart = engineDesc.noteSettings->noteRange.from;
      const int noteRangeEnd   = engineDesc.noteSettings->noteRange.to;
      if (engineDesc.noteSettings->midi)
      {
         if (engineDesc.noteSettings->midi->pitchRouting)
         {
            const float value =
                engineDesc.noteSettings->midi->pitchRouting->mapping
                    ? engineDesc.noteSettings->midi->pitchRouting->mapping->
                      operator[](note - noteRangeStart)
                    : note / 127.0f;

            sendSoundParameter(voiceIndex,
                               engineDesc.noteSettings->midi->pitchRouting
                                   ->destinationParameterIdx,
                               value);
         }
         if (engineDesc.noteSettings->midi->velocityRouting)
         {
            const float value =
                engineDesc.noteSettings->midi->velocityRouting->mapping
                    ? engineDesc.noteSettings->midi->velocityRouting->mapping->
                      operator[](note - noteRangeStart)
                    : velocity;
            sendSoundParameter(voiceIndex,
                               engineDesc.noteSettings->midi->velocityRouting
                                   ->destinationParameterIdx,
                               value);
         }
      }
   }
   const int note2Send = voiceDescr.midiTriggerNoteNumber
                             ? *voiceDescr.midiTriggerNoteNumber
                             : note;
   m_pMidiOutIf->noteOn(voiceDescr.midiChannel + m_midiChannelOffset, note2Send,
                        velocity * 127);
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::noteOff(int voiceIndex, int note,
                                                     float velocity) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   const int note2Send    = voiceDescr.midiTriggerNoteNumber
                                ? *voiceDescr.midiTriggerNoteNumber
                                : note;

   m_pMidiOutIf->noteOff(voiceDescr.midiChannel + m_midiChannelOffset,
                         note2Send, velocity * 127);
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::pitchBend(int voiceIndex,
                                                       float value) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   m_pMidiOutIf->pitchBend(voiceDescr.midiChannel + m_midiChannelOffset,
                           value * m_pitchBendFactor * 16383);
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::afterTouchPoly(
    int voiceIndex, int note, float value) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   m_pMidiOutIf->afterTouchPoly(
       voiceDescr.midiChannel + m_midiChannelOffset + m_midiChannelOffset, note,
       value * 127);
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::afterTouch(int voiceIndex,
                                                        float value) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   m_pMidiOutIf->afterTouch(voiceDescr.midiChannel + m_midiChannelOffset,
                            value * 127);
}

template <typename MidiOutIfPtr>
void sound::MidiOutMsgHandler<MidiOutIfPtr>::programChange(
    int voiceIndex, int programIdx) noexcept
{
   assert(base::musicDevice::description::sound::GlobalSectionId != voiceIndex);
   const auto& voiceDescr = m_rSoundSection.voices[voiceIndex];
   m_pMidiOutIf->send(midi::Message<midi::ProgramChange>(
       voiceDescr.midiChannel + m_midiChannelOffset, programIdx));
}

template <typename MidiOutIfPtr>
uint8_t sound::MidiOutMsgHandler<MidiOutIfPtr>::getMidiChannelOffset()
    const noexcept
{
   return m_midiChannelOffset;
}

}   // namespace base::musicDevice