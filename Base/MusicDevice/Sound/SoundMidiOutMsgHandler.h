#ifndef SOUND_MIDI_OUT_MSG_HANDLER_H
#define SOUND_MIDI_OUT_MSG_HANDLER_H

#include "SoundSection.h"

namespace base::musicDevice
{
namespace sound
{
template <typename MidiOutIfPtr> class MidiOutMsgHandler
{
public:
   MidiOutMsgHandler(MidiOutIfPtr rMidiOutIf,
                     const description::sound::Section& rSoundSection,
                     uint8_t midiChannelOffset) noexcept;
   void sendSoundParameter(uint32_t voiceId, uint32_t parameterId,
                           float value) noexcept;
   void sendParameterDumpRequest() noexcept;

   void noteOn(int voiceIdx, int note, float velocity) noexcept;
   void noteOff(int voiceIdx, int note, float velocity) noexcept;
   void pitchBend(int voiceIdx, float value) noexcept;
   void afterTouchPoly(int voiceIdx, int note, float value) noexcept;
   void afterTouch(int voiceIdx, float value) noexcept;
   void programChange(int voiceIdx, int programIdx) noexcept;

   uint8_t getMidiChannelOffset() const noexcept;

private:
   MidiOutIfPtr m_pMidiOutIf;
   const description::sound::Section& m_rSoundSection;
   float m_semitonesPerHalfPitchbendRange{2.0};
   uint8_t m_midiChannelOffset{0};
};

}   // namespace sound
}   // namespace base::musicDevice

#include "SoundMidiOutMsgHandler.inl"

#endif