#ifndef SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H
#define SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H

#include <functional>

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
   void handle(const midi::Message<midi::SystemExclusive>& sysexMsg) noexcept;

private:
   const description::sound::Section& m_rSoundSection;
   uint8_t m_midiVoiceOffset{0};
   Cb m_drainCb;
   bool checkIfIsParameterDumpMsg(
       const midi::Message<midi::SystemExclusive>& sysexMsg) const noexcept;
   std::optional<int> getVoiceIdFromSysex(
       const midi::Message<midi::SystemExclusive>& sysexMsg) const noexcept;
};

}   // namespace base::musicDevice::sound

#endif   // SOUND_MIDI_IN_SYSEX_DUMP_HANDLER_H