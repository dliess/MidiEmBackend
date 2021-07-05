#include "RtServer.h"

#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // meta::serialize
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "SoundDevicesRpc.h"
#include "TempoRpc.h"
#include "TransportControl.h"
#include "TransportControlRpc.h"
#include "MidiRoutingRpc.h"

using namespace uiadapter::capnzero;

RtServer::RtServer(
    zmq::context_t &rZmqContext, base::instruments::Instruments &rInstruments,
    base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
    base::musicDevice::TransportControl &rTransportControl) :
    ::capnzero::MidiEmRt::MidiEmRtServer(
        rZmqContext, "tcp://*:5555", "tcp://*:5556",
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<SoundDevicesRpc>(rMusicDeviceContainer),
        std::make_unique<TempoRpc>(Super::signals()),
        std::make_unique<TransportControlRpc>(rTransportControl),
        std::make_unique<MidiRoutingRpc>())
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
       [&rMusicDeviceContainer, &rInstruments, &rTransportControl](Signals &rSignals) {
          for (auto &it : rMusicDeviceContainer)
          {
             const auto uuid        = it.second.get()->id();
             const auto &deviceName = it.second.get()->deviceId().deviceName;
             const auto &portName   = it.second.get()->deviceId().portName;
             const auto midiVoiceOffset =
                 it.second.get()->soundHandler
                     ? it.second.get()->soundHandler->getMidiVoiceOffset()
                     : 0;
             const base::musicDevice::description::Description &description =
                 *it.second.get()->description();
             rSignals.MusicDevices__musicDeviceDescriptionAdded(
                 deviceName, meta::serialize(description).dump().c_str());
             rSignals.MusicDevices__deviceAdded(uuid, deviceName, portName,
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
       });

   rMusicDeviceContainer.registerForAdd(
       [this, &rMusicDeviceContainer](
           std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          const auto &deviceName  = ptr.get()->deviceId().deviceName;
          const auto &description = *ptr.get()->description();
          const auto midiVoiceOffset =
              ptr.get()->soundHandler
                  ? ptr.get()->soundHandler->getMidiVoiceOffset()
                  : 0;

          signals().MusicDevices__musicDeviceDescriptionAdded(
              deviceName, meta::serialize(description).dump().c_str());

          signals().MusicDevices__deviceAdded(
              ptr.get()->id(), ptr.get()->deviceId().deviceName,
              ptr.get()->deviceId().portName, midiVoiceOffset);
       });

   rMusicDeviceContainer.registerForAboutToRemove(
       [this, &rMusicDeviceContainer](
           std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          signals().MusicDevices__deviceRemoved(ptr.get()->id());
       });

   rMusicDeviceContainer.registerSoundDevParamChangeCbUI(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float commanded, float actual) {
          signals().SoundDevices__parameterChanged(uuid, voiceId, paramIdx,
                                                   commanded, actual);
       });

   rTransportControl.registerTransportMaskChangedCb(
       [this](const util::Identifiable::UUID &uuid, bool masked) {
          signals().TransportControl__enabledChanged(uuid, !masked);
       });
    rTransportControl.registerStartedChangeNotifCb([this](bool started){
        signals().TransportControl__startedChanged(started ? 1 : 0);
    });
}