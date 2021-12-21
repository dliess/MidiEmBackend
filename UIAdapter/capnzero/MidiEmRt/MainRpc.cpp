#include "MainRpc.h"

using namespace uiadapter::capnzero;

MainRpc::MainRpc(RtServer::Signals &rSignals,
                 base::instruments::Instruments &rInstruments,
                 base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
                 base::musicDevice::TransportControl &rTransportControl,
                 base::midifriends::Router &rMidiRouter) :
    m_rSignals(rSignals),
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer),
    m_rTransportControl(rTransportControl),
    m_rMidiRouter(rMidiRouter)
{
}

void MainRpc::reEmitSignals()
{
   LOG_F(INFO, "MainRpc::reEmitSignals() called");
   for (auto &it : m_rMusicDeviceContainer)
   {
      const auto uuid        = it.second.get()->id();
      const auto &deviceName = it.second.get()->deviceId().deviceName;
      const auto &portName   = it.second.get()->deviceId().portName;
      const auto mediumId    = it.second.get()->mediumId();
      const auto midiVoiceOffset =
          it.second.get()->soundHandler
              ? it.second.get()->soundHandler->getMidiVoiceOffset()
              : 0;
      const base::musicDevice::description::Description &description =
          *it.second.get()->description();
      m_rSignals.MusicDevices__deviceAdded(uuid, deviceName, portName,
                                           mediumId.toStr(), midiVoiceOffset);
      if (it.second.get()->soundHandler)
      {
         auto &arpeggiators = it.second.get()->soundHandler->arpeggiators();
         for (int voiceIdx = 0; voiceIdx < arpeggiators.size(); ++voiceIdx)
         {
            m_rSignals.SoundDevices__arpeggiatorBypassChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getBypass());
            m_rSignals.SoundDevices__arpeggiatorRangeTypeChanged(
                uuid, voiceIdx,
                static_cast<::capnzero::MidiEmRt::ArpeggiatorRangeType>(
                    arpeggiators.at(voiceIdx).getRangeType()));
            m_rSignals.SoundDevices__arpeggiatorRangeChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getRange());
            m_rSignals.SoundDevices__arpeggiatorGateFillChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getGateFill());
            m_rSignals.SoundDevices__arpeggiatorStepLengthChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getStepLength());
            m_rSignals.SoundDevices__arpeggiatorAlgorithmChanged(
                uuid, voiceIdx,
                static_cast<::capnzero::MidiEmRt::ArpeggiatorAlgorithm>(
                    arpeggiators.at(voiceIdx).getAlgorithm()));
            m_rSignals.SoundDevices__arpeggiatorHoldNotesChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getHoldNotes());
            m_rSignals.SoundDevices__arpeggiatorFeedModeChanged(
                uuid, voiceIdx,
                static_cast<::capnzero::MidiEmRt::ArpeggiatorFeedMode>(
                    arpeggiators.at(voiceIdx).getFeedMode()));
            m_rSignals.SoundDevices__arpeggiatorSeqSizeChanged(
                uuid, voiceIdx, arpeggiators.at(voiceIdx).getSeqSize());
            const auto actualPresetName =
                it.second.get()->soundHandler->getActualPresetOfVoice(voiceIdx);
            if (actualPresetName && !actualPresetName->empty())
            {
               m_rSignals.SoundDevices__actualPresetChanged(uuid, voiceIdx,
                                                            *actualPresetName);
            }
         }
      }
   }
   m_rSignals.Tempo__beatTickStartedChanged(
       base::tempo::BeatTick::instance().running());
   m_rSignals.Tempo__bpmCentsChanged(
       base::tempo::BeatTick::instance().getBpmCentsNudged());
   m_rSignals.Instruments__kitInstrumentsChanged(
       meta::serialize(m_rInstruments.data.kitInstruments).dump().c_str());
   m_rSignals.Instruments__melodicInstrumentsChanged(
       meta::serialize(m_rInstruments.data.melodicInstruments).dump().c_str());
   m_rTransportControl.retriggerCallbacks();
   m_rMidiRouter.retriggerCallbacks();
}
