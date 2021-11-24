#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext, LoaderServer::Signals& rSignals,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext,
                                            "tcp://localhost:5555"),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext,
                                                "tcp://localhost:5556")
{
   onSoundDevicesPresetChanged(
       [this, &rMDFactory](const ::capnzero::TextView& deviceName,
                           ::capnzero::Int8 engineIdx,
                           const ::capnzero::TextView& presetName) {
          const sound::preset::Id presetId(
              {std::string(deviceName), engineIdx, std::string(presetName)});
          rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
       });

   onMusicDevicesDeviceAdded([&rSignals](const ::capnzero::SpanCL<16>& uuid,
                                         const ::capnzero::TextView& type,
                                         const ::capnzero::TextView& port,
                                         const ::capnzero::TextView& mediumId,
                                         ::capnzero::UInt8 midiVoiceOffset) {
      ::capnzero::Data<16> uuidData;
      std::copy(uuid.begin(), uuid.end(), uuidData.begin());
      rSignals.MusicDevices__deviceAdded(
          uuidData, std::string(type), std::string(port), std::string(mediumId),
          midiVoiceOffset);
   });
   onMusicDevicesDeviceRemoved([&rSignals](const ::capnzero::SpanCL<16>& uuid) {
      ::capnzero::Data<16> uuidData;
      std::copy(uuid.begin(), uuid.end(), uuidData.begin());
      rSignals.MusicDevices__deviceRemoved(uuidData);
   });

   onSoundDevicesActualPresetChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx,
                   const ::capnzero::TextView& presetName) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__actualPresetChanged(uuidData, voiceIdx,
                                                     std::string(presetName));
       });

   onSoundDevicesArpeggiatorAlgorithmChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx,
                   ::capnzero::MidiEmRt::ArpeggiatorAlgorithm algorithm) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorAlgorithmChanged(
              uuidData, voiceIdx,
              static_cast<::capnzero::MidiEmLoader::ArpeggiatorAlgorithm>(
                  algorithm));
       });
   onSoundDevicesArpeggiatorBypassChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int8 on) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorBypassChanged(uuidData, voiceIdx,
                                                          on);
       });

   onSoundDevicesArpeggiatorGateFillChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Float32 gateFill) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorGateFillChanged(uuidData, voiceIdx,
                                                            gateFill);
       });
   onSoundDevicesArpeggiatorHoldNotesChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int8 on) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorHoldNotesChanged(uuidData, voiceIdx,
                                                             on);
       });
   onSoundDevicesArpeggiatorRangeChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int32 range) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorRangeChanged(uuidData, voiceIdx,
                                                         range);
       });
   onSoundDevicesArpeggiatorRangeTypeChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx,
                   ::capnzero::MidiEmRt::ArpeggiatorRangeType rangeType) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorRangeTypeChanged(
              uuidData, voiceIdx,
              static_cast<::capnzero::MidiEmLoader::ArpeggiatorRangeType>(
                  rangeType));
       });
   onSoundDevicesArpeggiatorStepLengthChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int32 stepLength) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorStepLengthChanged(uuidData, voiceIdx,
                                                          stepLength);
       });

   onTransportControlEnabledChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::UInt8 enabled) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.TransportControl__enabledChanged(uuidData, enabled);
       });
}