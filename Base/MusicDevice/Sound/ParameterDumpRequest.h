#ifndef SOUND_PARAMETER_DUMP_REQUEST_H
#define SOUND_PARAMETER_DUMP_REQUEST_H

#include "SoundSection.h"

namespace base::musicDevice::sound
{
template <typename MidiOutIf> class ParameterDumpRequest
{
public:
   ParameterDumpRequest(MidiOutIf& rMidiOutIf,
                        const description::sound::Section& rSoundSection,
                        int midiChannelOffset = 0) noexcept;
   void sendParameterDumpRequest() noexcept;
   // Attention: it can happen that the dump comes back for all voices (like octatrack)
   void sendParameterDumpRequest(int voiceIdx) noexcept;

private:
   MidiOutIf& m_rMidiOutIf;
   const description::sound::Section& m_rSoundSection;
   int m_midiChannelOffset{0};

   static constexpr int ALL_VOICES = -2;
   void _sendParameterDumpRequest(
       const description::sound::ParameterDumpRequest&
           parameterDumpRequest, int voiceIdx = ALL_VOICES) noexcept;
};

}   // namespace base::musicDevice::sound

#include "ParameterDumpRequest.inl"

#endif