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
   m_presetName     = std::nullopt;
   m_presetCategory = std::nullopt;
   m_presetGenre    = std::nullopt;
   if (!m_rSoundSection.parameterDumpAnswer)
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
        m_rSoundSection.parameterDumpAnswer->sysexDescriptors)
   {
      mpark::visit(
          util::overload{
              [this, &voiceIdx, &sysexMsg](
                  const description::sound::midisysex::ParameterLowRes& param) {
                 const auto paramIdx = m_rSoundSection.getParameterIdx(
                     *voiceIdx, param.component, param.parameter);
                 if (!paramIdx)
                 {
                    LOG_F(ERROR,
                          "There is no parameter in voice {} named {}::{}",
                          *voiceIdx, param.component, param.parameter);
                    return;
                 }
                 const auto& descr =
                     m_rSoundSection.parameterDescr(*voiceIdx, *paramIdx);
                 assert(descr.source.midi.has_value());
                 const int value = sysexMsg[param.offset];
                 if (descr.type == description::sound::Parameter::Type::List)
                 {
                    m_drainCb(*voiceIdx, *paramIdx,
                              descr.getListIndexByValue(value));
                 }
                 else
                 {
                    const description::sound::ValueRangeInt valueRange =
                        descr.source.midi->sourceValueRange
                            ? *descr.source.midi->sourceValueRange
                            : description::sound::ValueRangeInt({0, 127});
                    m_drainCb(*voiceIdx, *paramIdx,
                              (value - valueRange.from) /
                                  float(valueRange.to - valueRange.from + 1));
                 }
              },
              [this, &sysexMsg](const description::sound::midisysex::PatchNameStr& patchName){
                  m_presetName = std::string(sysexMsg[patchName.offset], patchName.size);
              },
              [this, &sysexMsg](const description::sound::midisysex::PatchCategory& patchCategory){
                  m_presetCategory = static_cast<preset::Category>(sysexMsg[patchCategory.offset]);
              },
              [this, &sysexMsg](const description::sound::midisysex::PatchGenre& patchGenre){
                   m_presetGenre = static_cast<preset::Genre>(sysexMsg[patchGenre.offset]);
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
        m_rSoundSection.parameterDumpAnswer->sysexDescriptors)
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
        m_rSoundSection.parameterDumpAnswer->sysexDescriptors)
   {
      std::optional<int> ret = mpark::visit(
          util::overload{
              [this, &sysexMsg](
                  const description::sound::midisysex::VoiceIdx& voiceIdx)
                  -> std::optional<int> {
                 const int idx = sysexMsg[voiceIdx.offset];
                 if (!util::vector_index_in_range(idx, voiceIdx.mapping))
                 {
                    return std::nullopt;
                 }
                 return voiceIdx.mapping[idx];
              },
              [](auto&& other) -> std::optional<int> { return std::nullopt; }},
          fieldDescr);
      if (ret)
         return ret;
   }
   return std::nullopt;
}

std::optional<std::string> MidiInSysExDumpHandler::presetName() const noexcept
{
   return m_presetName;
}

std::optional<preset::Category> MidiInSysExDumpHandler::presetCategory()
    const noexcept
{
   return m_presetCategory;
}

std::optional<preset::Genre> MidiInSysExDumpHandler::presetGenre()
    const noexcept
{
   return m_presetGenre;
}
