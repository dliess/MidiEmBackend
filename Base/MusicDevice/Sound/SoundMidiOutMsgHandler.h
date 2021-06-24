#ifndef SOUND_MIDI_OUT_MSG_HANDLER_H
#define SOUND_MIDI_OUT_MSG_HANDLER_H

#include "SoundSection.h"

namespace base::musicDevice
{
namespace sound
{
template <typename MidiOutIfPtr>
class MidiOutMsgHandler
{
public:
   MidiOutMsgHandler(MidiOutIfPtr rMidiOutIf,
                     const description::sound::Section& rSoundSection,
                     uint8_t midiChannelOffset) noexcept;
   void sendSoundParameter(uint32_t voiceId, uint32_t parameterId,
                           float value) noexcept;
   bool sendParameterDumpRequest() noexcept;

   void noteOn(int voiceIndex, int note, float velocity) noexcept;
   void noteOff(int voiceIndex, int note, float velocity) noexcept;
   void pitchBend(int voiceIndex, float value) noexcept;
   void afterTouchPoly(int voiceIndex, int note, float value) noexcept;
   void afterTouch(int voiceIndex, float value) noexcept;

   uint8_t getMidiChannelOffset() const noexcept;

private:
   MidiOutIfPtr m_pMidiOutIf;
   const description::sound::Section& m_rSoundSection;
   float m_pitchBendFactor{1.0};
   uint8_t m_midiChannelOffset{0};
};

} // namespace sound
} // namespace base::musicDevice::sound

#include "SoundMidiOutMsgHandler.tcc"

#endif