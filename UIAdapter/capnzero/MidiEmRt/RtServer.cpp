#include "RtServer.h"

#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // meta::serialize
#include "MidiRouter.h"
#include "MidiRoutingRpc.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "SoundDevicesRpc.h"
#include "TempoRpc.h"
#include "TransportControl.h"
#include "TransportControlRpc.h"

using namespace uiadapter::capnzero;

RtServer::RtServer(
    zmq::context_t &rZmqContext, base::instruments::Instruments &rInstruments,
    base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
    base::musicDevice::TransportControl &rTransportControl,
    base::midifriends::Router &rMidiRouter) :
    ::capnzero::MidiEmRt::MidiEmRtServer(
        rZmqContext, "tcp://*:5555", "tcp://*:5556",
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<SoundDevicesRpc>(rMusicDeviceContainer),
        std::make_unique<TempoRpc>(Super::signals()),
        std::make_unique<TransportControlRpc>(rTransportControl),
        std::make_unique<MidiRoutingRpc>(rMidiRouter))
{
   rInstruments.registerForDataChange([this, &rInstruments]() {
      Super::signals().Instruments__kitInstrumentsChanged(
          meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
      Super::signals().Instruments__melodicInstrumentsChanged(
          meta::serialize(rInstruments.data.melodicInstruments).dump().c_str());
   });

   // ATTENTION: to send all initial subscription data in ordered manner,
   // we use only one of the Subscription callbacks, since we dont know
   // the call order otherwise
   Super::signals().registerMusicDevicesDeviceAddedSubscrCb(
       [&rMusicDeviceContainer, &rInstruments, &rTransportControl,
        &rMidiRouter](Signals &rSignals) {
          for (auto &it : rMusicDeviceContainer)
          {
             const auto uuid        = it.second.get()->id();
             const auto &deviceName = it.second.get()->deviceId().deviceName;
             const auto &portName   = it.second.get()->deviceId().portName;
             const auto mediumId    = it.second.get()->mediumId();
             assert(mediumId.has_value());
             const auto midiVoiceOffset =
                 it.second.get()->soundHandler
                     ? it.second.get()->soundHandler->getMidiVoiceOffset()
                     : 0;
             const base::musicDevice::description::Description &description =
                 *it.second.get()->description();
             rSignals.MusicDevices__deviceAdded(uuid, deviceName, portName,
                                                mediumId->toStr(),
                                                midiVoiceOffset);
          }
          rSignals.Tempo__beatTickStartedChanged(
              base::tempo::BeatTick::instance().running());
          rSignals.Tempo__bpmCentsChanged(
              base::tempo::BeatTick::instance().getBpmCents());
          rSignals.Instruments__kitInstrumentsChanged(
              meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
          rSignals.Instruments__melodicInstrumentsChanged(
              meta::serialize(rInstruments.data.melodicInstruments)
                  .dump()
                  .c_str());
          rTransportControl.retriggerCallbacks();
          rMidiRouter.retriggerCallbacks();
       });

   rMusicDeviceContainer.registerForAdd(
       [this](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          const auto &deviceName  = ptr.get()->deviceId().deviceName;
          const auto &description = *ptr.get()->description();
          const auto &mediumId    = ptr.get()->mediumId();
          assert(mediumId.has_value());
          const auto midiVoiceOffset =
              ptr.get()->soundHandler
                  ? ptr.get()->soundHandler->getMidiVoiceOffset()
                  : 0;

          signals().MusicDevices__deviceAdded(
              ptr.get()->id(), ptr.get()->deviceId().deviceName,
              ptr.get()->deviceId().portName, mediumId->toStr(),
              midiVoiceOffset);
       });

   rMusicDeviceContainer.registerForAboutToRemove(
       [this](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          signals().MusicDevices__deviceRemoved(ptr.get()->id());
       });

   rMusicDeviceContainer.registerSoundDevParamChangeCbUI(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float commanded, float actual) {
          signals().SoundDevices__parameterChanged(uuid, voiceId, paramIdx,
                                                   commanded, actual);
       });

   rMusicDeviceContainer.registerLFOWaveformChangeCB(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              base::musicDevice::sound::lfo::Waveform waveform) {
          signals().SoundDevices__lFOWaveformChanged(
              uuid, voiceId, paramIdx,
              static_cast<::capnzero::MidiEmRt::LFOWaveform>(waveform));
       });

   rMusicDeviceContainer.registerLFOAmplitudeChangeCB(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float amplitude) {
          signals().SoundDevices__lFOAmplitudeChanged(uuid, voiceId, paramIdx,
                                                      amplitude);
       });

   rMusicDeviceContainer.registerLFOFrequencyChangeCB(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float frequency) {
          signals().SoundDevices__lFOFrequencyChanged(uuid, voiceId, paramIdx,
                                                      frequency);
       });

   rMusicDeviceContainer.registerLFOMultiplierExpChangeCB(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              uint32_t multiplierExp) {
          signals().SoundDevices__lFOMultiplierExpChanged(
              uuid, voiceId, paramIdx, multiplierExp);
       });

   rMusicDeviceContainer.registerEnginePresetChangeCB(
       [this](const std::string &musicDeviceName, int engineIdx,
              const std::string &presetName) {
          signals().SoundDevices__presetChanged(musicDeviceName, engineIdx, presetName);
       });

   rTransportControl.registerTransportMaskChangedCb(
       [this](const util::Identifiable::UUID &uuid, bool masked) {
          signals().TransportControl__enabledChanged(uuid, !masked);
       });
   rTransportControl.registerStartedChangeNotifCb([this](bool started) {
      signals().TransportControl__startedChanged(started ? 1 : 0);
   });

   rMidiRouter.registerRoutedChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest, bool routed) {
          if (routed)
          {
             signals().MidiRouting__routedAdded(source.toStr(), dest.toStr());
          }
          else
          {
             signals().MidiRouting__routedRemoved(source.toStr(), dest.toStr());
          }
       });
   rMidiRouter.registerSpecialRoutedChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest, bool created) {
          if (created)
          {
             signals().MidiRouting__specializedRoutingInited(source.toStr(),
                                                             dest.toStr());
          }
          else
          {
             signals().MidiRouting__specializedRoutingCleared(source.toStr(),
                                                              dest.toStr());
          }
       });
   rMidiRouter.registerSpecialRouteChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest,
              uint8_t sourceChannel, uint8_t destChannel, bool enabled) {
          signals().MidiRouting__specializedRoutingSet(
              source.toStr(), dest.toStr(), sourceChannel, destChannel,
              enabled);
       });
}