#include "RtServer.h"

#include "AbletonLinkRpc.h"
#include "AbletonLinkWrapper.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // meta::serialize
#include "MainRpc.h"
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
    base::AbletonLinkWrapper &rAbletonLinkWrapper,
    base::midifriends::Router &rMidiRouter) :
    ::capnzero::MidiEmRt::MidiEmRtServer(
        rZmqContext, "tcp://*:55555", "tcp://*:55556",
        std::make_unique<MainRpc>(signals(), rInstruments,
                                  rMusicDeviceContainer, rTransportControl,
                                  rAbletonLinkWrapper, rMidiRouter),
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<SoundDevicesRpc>(rMusicDeviceContainer),
        std::make_unique<TempoRpc>(Super::signals()),
        std::make_unique<TransportControlRpc>(rTransportControl),
        std::make_unique<AbletonLinkRpc>(rAbletonLinkWrapper),
        std::make_unique<MidiRoutingRpc>(rMidiRouter))
{
   rInstruments.registerForDataChange([this, &rInstruments]() {
      Super::signals().Instruments__kitInstrumentsChanged(
          meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
      Super::signals().Instruments__melodicInstrumentsChanged(
          meta::serialize(rInstruments.data.melodicInstruments).dump().c_str());
   });

   rMusicDeviceContainer.onAboutToAdd(
       [this](const base::musicDevice::MusicDevice& md) {
          const auto &deviceName  = md.deviceId().deviceName;
          const auto &description = *md.description();
          const auto &mediumId    = md.mediumId();
          const auto midiVoiceOffset =
              md.soundHandler
                  ? md.soundHandler->getMidiVoiceOffset()
                  : 0;

          signals().MusicDevices__deviceAdded(
              md.id(), md.deviceId().deviceName,
              md.deviceId().portName, mediumId.toStr(),
              midiVoiceOffset);
       });

   rMusicDeviceContainer.onAboutToRemove(
       [this](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          signals().MusicDevices__deviceRemoved(ptr.get()->id());
       });

   rMusicDeviceContainer.onSoundDevParamChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float commanded, float actual) {
          signals().SoundDevices__parameterChanged(uuid, voiceId, paramIdx,
                                                   commanded, actual);
       });

   rMusicDeviceContainer.onLFOWaveformChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              base::musicDevice::sound::lfo::Waveform waveform) {
          signals().SoundDevices__lFOWaveformChanged(
              uuid, voiceId, paramIdx,
              static_cast<::capnzero::MidiEmRt::LFOWaveform>(waveform));
       });

   rMusicDeviceContainer.onLFOAmplitudeChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float amplitude) {
          signals().SoundDevices__lFOAmplitudeChanged(uuid, voiceId, paramIdx,
                                                      amplitude);
       });

   rMusicDeviceContainer.onLFOFrequencyChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float frequency) {
          signals().SoundDevices__lFOFrequencyChanged(uuid, voiceId, paramIdx,
                                                      frequency);
       });

   rMusicDeviceContainer.onLFOMultiplierExpChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              uint32_t multiplierExp) {
          signals().SoundDevices__lFOMultiplierExpChanged(
              uuid, voiceId, paramIdx, multiplierExp);
       });

   rMusicDeviceContainer.onEnginePresetChanged(
       [this](const std::string &musicDeviceName, int engineIdx,
              const std::string &presetName) {
          signals().SoundDevices__presetChanged(musicDeviceName, engineIdx,
                                                presetName);
       });
   // Actual Preset -----------------
   rMusicDeviceContainer.onActualPresetChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              const std::string &presetName) {
          signals().SoundDevices__actualPresetChanged(uuid, voiceIdx,
                                                      presetName);
       });

   // ARP -------------
   rMusicDeviceContainer.onArpBypassChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, bool on) {
          signals().SoundDevices__arpeggiatorBypassChanged(uuid, voiceIdx, on);
       });
   rMusicDeviceContainer.onArpRangeTypeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::RangeType rangeType) {
          signals().SoundDevices__arpeggiatorRangeTypeChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorRangeType>(
                  rangeType));
       });
   rMusicDeviceContainer.onArpRangeChanged([this](util::Identifiable::UUID uuid,
                                                  int voiceIdx, int range) {
      signals().SoundDevices__arpeggiatorRangeChanged(uuid, voiceIdx, range);
   });
   rMusicDeviceContainer.onArpGateFillChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, float gateFill) {
          signals().SoundDevices__arpeggiatorGateFillChanged(uuid, voiceIdx,
                                                             gateFill);
       });
   rMusicDeviceContainer.onArpStepLengthChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, float stepLength) {
          signals().SoundDevices__arpeggiatorStepLengthChanged(uuid, voiceIdx,
                                                               stepLength);
       });
   rMusicDeviceContainer.onArpAlgorithmChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::Algorithm algorithm) {
          signals().SoundDevices__arpeggiatorAlgorithmChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorAlgorithm>(
                  algorithm));
       });
   rMusicDeviceContainer.onArpHoldNotesChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, bool holdNotes) {
          signals().SoundDevices__arpeggiatorHoldNotesChanged(uuid, voiceIdx,
                                                              holdNotes);
       });
   rMusicDeviceContainer.onArpFeedModeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::FeedMode feedMode) {
          signals().SoundDevices__arpeggiatorFeedModeChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorFeedMode>(feedMode));
       });
   rMusicDeviceContainer.onArpSeqSizeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, int seqSize) {
          signals().SoundDevices__arpeggiatorSeqSizeChanged(uuid, voiceIdx,
                                                            seqSize);
       });
   // -----------------

   rTransportControl.onTransportMaskChanged(
       [this](const util::Identifiable::UUID &uuid, bool masked) {
          signals().TransportControl__enabledChanged(uuid, !masked);
       });
   rTransportControl.onStartedChanged([this](bool started) {
      signals().TransportControl__startedChanged(started ? 1 : 0);
   });
   rTransportControl.onStartOnBeat([this](bool startOnBeat) {
      signals().TransportControl__quantizedStartChanged(startOnBeat);
   });

   rAbletonLinkWrapper.onEnabledChanged([this](bool enabled) {
      signals().AbletonLink__enabledChanged(enabled);
   });

   rAbletonLinkWrapper.onReactsOnTransportChanged([this](bool reacts) {
      signals().AbletonLink__reactOnTransportChanged(reacts);
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