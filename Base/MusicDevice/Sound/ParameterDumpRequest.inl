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
                       sendParameterDumpRequest(voiceIdx);
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
                          sendParameterDumpRequest(voiceIdx);
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
   mpark::visit(
       util::overload{
           [this, voiceIdx](const description::sound::MidiCCAndValue& ccMsg) {
              m_rMidiOutIf.controlParameter(
                  m_rSoundSection.midiChannel(voiceIdx)->midiChannel +
                      m_midiChannelOffset,
                  ccMsg.cc[0], ccMsg.value);
           },
           [this, voiceIdx](const description::sound::MidiSysexMsg& sysExMsg) {
              std::vector<uint8_t> sysexMsgToSend;
              for (const auto& sysExDescr : sysExMsg.sysexDescriptors)
              {
                 mpark::visit(
                     util::overload{
                         [&sysexMsgToSend](
                             const description::sound::midisysex::Bytes&
                                 bytes) {
                            sysexMsgToSend.insert(sysexMsgToSend.begin(),
                                                  bytes.values.begin(),
                                                  bytes.values.end());
                         },
                         [voiceIdx, &sysexMsgToSend](
                             const description::sound::midisysex::VoiceIdx&
                                 voiceIdDescr) {
                            bool found = false;
                            for (uint8_t i = 0; i < voiceIdDescr.mapping.size();
                                 ++i)
                            {
                               if (voiceIdDescr.mapping[i] == voiceIdx)
                               {
                                  sysexMsgToSend.push_back(i);
                                  found = true;
                               }
                            }
                            if(!found)
                            {
                               LOG_F(ERROR, "Could not determine what channel id to write into parameter dump sysex message");
                               sysexMsgToSend.push_back(0);
                            }
                         },
                         [](auto&& other) {}},
                     sysExDescr);
              }
              m_rMidiOutIf.sysEx(sysexMsgToSend);
           }},
       m_rSoundSection.engineBase(voiceIdx)->parameterDumpRequest->message);
}

}   // namespace base::musicDevice::sound

#endif