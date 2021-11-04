#include "SoundMidiInSysExDumpHandler.h"

#include <loguru.hpp>

using namespace base::musicDevice::sound;

MidiInSysExDumpHandler::MidiInSysExDumpHandler(
    const description::sound::Section& rSoundSection, uint8_t midiVoiceOffset,
    Cb cb) noexcept :
    m_rSoundSection(rSoundSection),
    m_midiVoiceOffset(midiVoiceOffset),
    m_drainCb(cb)
{
}

void MidiInSysExDumpHandler::handle(
    const midi::Message<midi::SystemExclusive>& sysexMsg) noexcept
{
   if (!m_rSoundSection.parameterDump)
   {
      return;
   }
   if (!checkIfIsParameterDumpMsg(sysexMsg))
   {
      return;
   }
   const auto voiceIdx = getVoiceIdFromSysex(sysexMsg);
   if (!voiceIdx)
   {
      LOG_F(ERROR, "Could not extract voiceId from sysex msg");
      return;
   }
   for (const auto& fieldDescr :
        m_rSoundSection.parameterDump->sysexDescriptors)
   {
      mpark::visit(
          util::overload{
              [this, &voiceIdx, &sysexMsg](
                  const description::sound::midisysex::ParameterLowRes& param) {
                 const auto& descr =
                     m_rSoundSection.parameterDescr(*voiceIdx, param.idx);
                 assert(descr.source.midi.has_value());
                 const int value = sysexMsg[param.offset];
                 if (descr.type == description::sound::Parameter::Type::List)
                 {
                    m_drainCb(*voiceIdx, param.idx,
                              descr.getListIndexByValue(value));
                 }
                 else
                 {
                    const description::sound::ValueRangeInt valueRange =
                        descr.source.midi->sourceValueRange
                            ? *descr.source.midi->sourceValueRange
                            : description::sound::ValueRangeInt({0, 127});
                    m_drainCb(*voiceIdx, param.idx,
                              (value - valueRange.from) /
                                  float(valueRange.to - valueRange.from + 1));
                 }
              },
              [](auto&& other) {}},
          fieldDescr);
   }
}

bool MidiInSysExDumpHandler::checkIfIsParameterDumpMsg(
    const midi::Message<midi::SystemExclusive>& sysexMsg) const noexcept
{
   int accumSize = 0;
   for (const auto& fieldDescr :
        m_rSoundSection.parameterDump->sysexDescriptors)
   {
      accumSize += mpark::visit(
          util::overload{[](auto&& val) -> int { return val.sizeInSysex(); }},
          fieldDescr);
   }

   return sysexMsg.size() == accumSize;
}

std::optional<int> MidiInSysExDumpHandler::getVoiceIdFromSysex(
    const midi::Message<midi::SystemExclusive>& sysexMsg) const noexcept
{
   for (const auto& fieldDescr :
        m_rSoundSection.parameterDump->sysexDescriptors)
   {
      std::optional<int> ret = mpark::visit(
          util::overload{
              [this, &sysexMsg](
                  const description::sound::midisysex::VoiceIdx& voiceIdx)
                  -> std::optional<int> { return sysexMsg[voiceIdx.offset]; },
              [](auto&& other) -> std::optional<int> { return std::nullopt; }},
          fieldDescr);
      if (ret)
         return ret;
   }
   return std::nullopt;
}
