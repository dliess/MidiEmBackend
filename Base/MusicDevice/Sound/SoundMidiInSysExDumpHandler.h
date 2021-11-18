#ifndef SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H
#define SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H

#include <functional>
#include <optional>

#include "DevicePresets.h"
#include "MidiMessage.h"
#include "SoundSection.h"

namespace base::musicDevice::sound
{
class MidiInSysExDumpHandler
{
public:
   using Cb = std::function<void(int voiceId, int parameterId, float value)>;
   MidiInSysExDumpHandler(const description::sound::Section& rSoundSection,
                          uint8_t midiVoiceOffset, Cb cb) noexcept;
   void handle(const midi::Message<midi::SystemExclusive>& sysexMsgconst,
               std::optional<int> engineIdx = std::nullopt) noexcept;

   [[nodiscard]] std::optional<std::string> presetName() const noexcept;
   [[nodiscard]] std::optional<preset::Category> presetCategory()
       const noexcept;
   [[nodiscard]] std::optional<preset::Genre> presetGenre() const noexcept;

private:
   const description::sound::Section& m_rSoundSection;
   uint8_t m_midiVoiceOffset{0};
   Cb m_drainCb;
   const description::sound::SysExDescriptors* determineSysexDescriptor(
       const midi::Message<midi::SystemExclusive>& sysexMsg,
       const std::optional<int>& engineIdx) const noexcept;
   static bool checkIfIsParameterDumpMsg(
       const midi::Message<midi::SystemExclusive>& sysexMsg,
       const description::sound::SysExDescriptors& sysexMsgDescriptor)
       noexcept;
   std::optional<int> getVoiceIdFromSysex(
       const midi::Message<midi::SystemExclusive>& sysexMsg,
       const description::sound::SysExDescriptors& sysexMsgDescriptors) const noexcept;

   std::optional<std::string> m_presetName;
   std::optional<preset::Category> m_presetCategory;
   std::optional<preset::Genre> m_presetGenre;
};

}   // namespace base::musicDevice::sound

#endif   // SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H