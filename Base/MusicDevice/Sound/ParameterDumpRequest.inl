#ifndef SOUND_PARAMETER_DUMP_REQUEST_INL
#define SOUND_PARAMETER_DUMP_REQUEST_INL

namespace base::musicDevice::sound
{
template <typename MidiOutIf>
ParameterDumpRequest<MidiOutIf>::ParameterDumpRequest(
    MidiOutIf& rMidiOutIf, const description::sound::Section& rSoundSection,
    int midiChannelOffset) noexcept :
    m_rMidiOutIf(rMidiOutIf),
    m_rSoundSection(rSoundSection),
    m_midiChannelOffset(midiChannelOffset)
{
}

template <typename MidiOutIf>
void ParameterDumpRequest<MidiOutIf>::sendParameterDumpRequest() noexcept
{
   if (m_rSoundSection.parameterDumpRequest)
   {
      _sendParameterDumpRequest(*m_rSoundSection.parameterDumpRequest);
   }
   if (m_rSoundSection.global && m_rSoundSection.global->parameterDumpRequest)
   {
      _sendParameterDumpRequest(*m_rSoundSection.global->parameterDumpRequest);
   }
   for (int i = 0; i < m_rSoundSection.voices.size(); ++i)
   {
      if (m_rSoundSection.voices[i].parameterDumpRequest)
      {
         _sendParameterDumpRequest(
             *m_rSoundSection.voices[i].parameterDumpRequest);
      }
   }
}

template <typename MidiOutIf>
void ParameterDumpRequest<MidiOutIf>::_sendParameterDumpRequest(
    const description::sound::ParameterDumpRequest&
        parameterDumpRequest) noexcept
{
   mpark::visit(
       util::overload{[this](const description::sound::MidiCCAndValue& ccMsg) {
                         m_rMidiOutIf.controlParameter(
                             1 + m_midiChannelOffset, ccMsg.cc[0], ccMsg.value);
                      },
                      [this](const description::sound::MidiSysexMsg& sysExMsg) {
                         m_rMidiOutIf.sysEx(sysExMsg.value);
                      }},
       parameterDumpRequest);
}

}   // namespace base::musicDevice::sound

#endif