#include "MainRpc.h"

#include "JsonCast.h"   // nlohmann::json
#include "MusicDeviceDescription.h"

using namespace uiadapter::capnzero;

MainRpc::MainRpc(
    RtServer::Signals &rSignals,
    base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
    base::TransportControl &rTransportControl,
    base::AbletonLinkWrapper &rAbletonLinkWrapper,
    base::midifriends::Router &rMidiRouter,
    base::musicDevice::sound::ParameterSceneContainer &rParameterSceneContainer,
    base::session::Tracks &rTracks,
    base::instruments::Instruments& rInstruments) :
    m_rSignals(rSignals),
    m_rMusicDeviceContainer(rMusicDeviceContainer),
    m_rTransportControl(rTransportControl),
    m_rAbletonLinkWrapper(rAbletonLinkWrapper),
    m_rMidiRouter(rMidiRouter),
    m_rParameterSceneContainer(rParameterSceneContainer),
    m_rTracks(rTracks),
    m_rInstruments(rInstruments)
{
}

void MainRpc::reEmitSignals()
{
   spdlog::info("MainRpc::reEmitSignals() called");
   for (auto &it : m_rMusicDeviceContainer)
   {
      const auto uuid        = it.second.get()->id();
      const auto deviceName = it.second.get()->deviceId().deviceName();
      const auto portName   = it.second.get()->deviceId().portName();
      const auto mediumId    = it.second.get()->mediumId();
      const auto midiVoiceOffset =
          it.second.get()->soundHandler
              ? it.second.get()->soundHandler->getMidiVoiceOffset()
              : 0;
      const base::musicDevice::description::Description &description =
          *it.second.get()->description();
      m_rSignals.MusicDevices__musicDeviceDescriptionAdded(
          deviceName, nlohmann::json(description).dump().c_str());
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
      if (it.second.get()->controllerHandler)
      {
         it.second.get()->controllerHandler->triggerUICallbacks(true);
      }
   }
   m_rSignals.Tempo__bpmChanged(
       base::tempo::BeatTick::instance().getBpmNudged());
   m_rTransportControl.retriggerCallbacks();
   m_rAbletonLinkWrapper.retriggerCallbacks();
   m_rMidiRouter.retriggerCallbacks();
   m_rParameterSceneContainer.retriggerCallbacks();
   m_rSignals.Session__tracksChanged(nlohmann::json(m_rTracks).dump());
   m_rInstruments.retriggerParameterCacheCallbacks();
}
