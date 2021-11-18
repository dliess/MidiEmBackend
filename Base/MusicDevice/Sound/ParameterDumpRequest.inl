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
   m_rSoundSection.forEachEngineBase(
       [this](int engineIdx,
              const description::sound::EngineBase& rEngineBase) {
          if (!rEngineBase.parameterDumpRequest)
             return;
          switch (rEngineBase.parameterDumpRequest->effect)
          {
             case description::sound::ParameterDumpRequest::Effect::PerVoice:
             {
                m_rSoundSection.forEachVoiceOfEngine(
                    engineIdx, [this](int voiceIdx, int engineIdx) {
                       _sendParameterDumpRequest(voiceIdx);
                    });
                break;
             }
             case description::sound::ParameterDumpRequest::Effect::
                 AllVoicesOfEngine:
             {
                bool sent = false;
                m_rSoundSection.forEachVoiceOfEngine(
                    engineIdx, [this, &sent](int voiceIdx, int engineIdx) {
                       if (!sent)
                       {
                          sent = true;
                          _sendParameterDumpRequest(voiceIdx);
                       }
                    });
                break;
             }
          }
       });
}

template <typename MidiOutIf>
void ParameterDumpRequest<MidiOutIf>::sendParameterDumpRequest(
    int voiceIdx) noexcept
{
   const auto* engineBase = m_rSoundSection.engineBase(voiceIdx);
   if (!engineBase)
   {
      LOG_F(ERROR, "engineBase is NULL");
      return;
   }
   _sendParameterDumpRequest(engineBase->parameterDumpRequest, voiceIdx);
}

template <typename MidiOutIf>
void ParameterDumpRequest<MidiOutIf>::_sendParameterDumpRequest(
    int voiceIdx) noexcept
{
   mpark::visit(
       util::overload{[this](const description::sound::MidiCCAndValue& ccMsg) {
                         m_rMidiOutIf.controlParameter(
                             1 + m_midiChannelOffset, ccMsg.cc[0], ccMsg.value);
                      },
                      [this](const description::sound::MidiSysexMsg& sysExMsg) {
                         std::vector<uint8_t> sysexMsgToSend;
                         for (const auto& sysExDescr :
                              sysExMsg.sysexDescriptors)
                         {
                            /*
                            mpark::visit(util::overload {
                               [&sysexMsgToSend](const
                            description::sound::midisysex::Bytes& bytes){
                                  sysexMsgToSend.insert(sysexMsgToSend.begin(),
                            bytes.values.begin(), bytes.values.end());
                               },
                               [](const description::sound::midisysex::VoiceIdx&
                            voiceIdx){},
                               [](auto&& default){}
                            }, sysExDescr);
                            */
                         }
                         m_rMidiOutIf.sysEx(sysexMsgToSend);
                      }},
       m_rSoundSection.engineBase(voiceIdx)->parameterDumpRequest->message);
}

}   // namespace base::musicDevice::sound

#endif