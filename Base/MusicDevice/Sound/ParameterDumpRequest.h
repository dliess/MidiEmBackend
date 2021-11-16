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

private:
   MidiOutIf& m_rMidiOutIf;
   const description::sound::Section& m_rSoundSection;
   int m_midiChannelOffset{0};

   void _sendParameterDumpRequest(
       const description::sound::ParameterDumpRequest&
           parameterDumpRequest) noexcept;
};

}   // namespace base::musicDevice::sound

#include "ParameterDumpRequest.inl"

#endif