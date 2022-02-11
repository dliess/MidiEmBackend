#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext, LoaderServer::Signals& rSignals,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext,
                                            "tcp://localhost:55555"),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext,
                                                "tcp://localhost:55556")
{
   onSoundDevicesPresetChanged(
       [this, &rMDFactory](const ::capnzero::TextView& deviceName,
                           ::capnzero::Int8 engineIdx,
                           const ::capnzero::TextView& presetName) {
          const sound::preset::Id presetId(
              {std::string(deviceName), engineIdx, std::string(presetName)});
          rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
       });

   onMusicDevicesDeviceAdded(
       [&rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid,
                                const ::capnzero::TextView& type,
                                const ::capnzero::TextView& port,
                                const ::capnzero::TextView& mediumId,
                                ::capnzero::UInt8 midiVoiceOffset) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().addUuid2MdId(
              uuidData, MusicDeviceId{std::string(type), std::string(port)});
          rSignals.MusicDevices__deviceAdded(
              uuidData, std::string(type), std::string(port),
              std::string(mediumId), midiVoiceOffset);
       });
   onMusicDevicesDeviceRemoved(
       [&rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().removeEntryForUuid(uuidData);
          rSignals.MusicDevices__deviceRemoved(uuidData);
       });

   onSoundDevicesParameterChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                   ::capnzero::Float32 commanded, ::capnzero::Float32 actual) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__parameterChanged(uuidData, voiceIdx, paramIdx,
                                                  commanded, actual);
       });

   onSoundDevicesActualPresetChanged(
       [this, &rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      const ::capnzero::TextView& presetName) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().soundDeviceActualPresetNameChanged(
              uuidData, voiceIdx, std::string(presetName));
       });

   onSoundDevicesLFOWaveformChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                   ::capnzero::MidiEmRt::LFOWaveform waveform) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__lFOWaveformChanged(
              uuidData, voiceIdx, paramIdx,
              static_cast<::capnzero::MidiEmLoader::LFOWaveform>(waveform));
       });
   onSoundDevicesLFOAmplitudeChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                   ::capnzero::Float32 amplitude) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__lFOAmplitudeChanged(uuidData, voiceIdx,
                                                     paramIdx, amplitude);
       });
   onSoundDevicesLFOFrequencyChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                   ::capnzero::Float32 frequency) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__lFOFrequencyChanged(uuidData, voiceIdx,
                                                     paramIdx, frequency);
       });
   onSoundDevicesLFOMultiplierExpChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                   ::capnzero::UInt32 multiplierExp) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__lFOMultiplierExpChanged(
              uuidData, voiceIdx, paramIdx, multiplierExp);
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
                   ::capnzero::Int8 voiceIdx, ::capnzero::Float32 stepLength) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorStepLengthChanged(
              uuidData, voiceIdx, stepLength);
       });
   onSoundDevicesArpeggiatorFeedModeChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx,
                   ::capnzero::MidiEmRt::ArpeggiatorFeedMode feedMode) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorFeedModeChanged(
              uuidData, voiceIdx,
              static_cast<::capnzero::MidiEmLoader::ArpeggiatorFeedMode>(
                  feedMode));
       });
   onSoundDevicesArpeggiatorSeqSizeChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx, ::capnzero::Int32 seqSize) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.SoundDevices__arpeggiatorSeqSizeChanged(uuidData, voiceIdx,
                                                           seqSize);
       });

   onTempoOffsetChanged([&rSignals](const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int32 offsetMs,
                                    ::capnzero::Float32 offsetBeats) {
      ::capnzero::Data<16> uuidData;
      std::copy(uuid.begin(), uuid.end(), uuidData.begin());
      rSignals.Tempo__offsetChanged(uuidData, offsetMs, offsetBeats);
   });

   onTransportControlEnabledChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::UInt8 enabled) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rSignals.TransportControl__enabledChanged(uuidData, enabled);
       });
   onMidiRoutingRoutedAdded([&rSignals](const ::capnzero::TextView& from,
                                        const ::capnzero::TextView& to) {
      rSignals.MidiRouting__routedAdded(std::string(from), std::string(to));
   });
   onMidiRoutingRoutedRemoved([&rSignals](const ::capnzero::TextView& from,
                                          const ::capnzero::TextView& to) {
      rSignals.MidiRouting__routedRemoved(std::string(from), std::string(to));
   });
   onMidiRoutingSpecializedRoutingInited(
       [&rSignals](const ::capnzero::TextView& from,
                   const ::capnzero::TextView& to) {
          rSignals.MidiRouting__specializedRoutingInited(std::string(from),
                                                         std::string(to));
       });
   onMidiRoutingSpecializedRoutingCleared(
       [&rSignals](const ::capnzero::TextView& from,
                   const ::capnzero::TextView& to) {
          rSignals.MidiRouting__specializedRoutingCleared(std::string(from),
                                                          std::string(to));
       });
   onMidiRoutingSpecializedRoutingSet(
       [&rSignals](const ::capnzero::TextView& from,
                   const ::capnzero::TextView& to,
                   ::capnzero::UInt8 sourceChannel,
                   ::capnzero::UInt8 destChannel, ::capnzero::UInt8 enable) {
          rSignals.MidiRouting__specializedRoutingSet(
              std::string(from), std::string(to), sourceChannel, destChannel,
              enable);
       });

    onAbletonLinkEnabledChanged([](::capnzero::UInt8 val){
        // TODO: save it to file and load
    });
    onAbletonLinkOffsetTimeUsChanged([](::capnzero::Int32 val){
        // TODO: save it to file and load
    });
}