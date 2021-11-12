#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   LoaderServer::Signals& rSignals,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext,
                                            "tcp://localhost:5555"),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext,
                                                "tcp://localhost:5556")
{
   onSoundDevicesPresetChanged(
       [this, &rMDFactory](const ::capnzero::TextView& deviceName,
                           ::capnzero::Int8 engineIdx,
                           const ::capnzero::TextView& presetName)
       {
          const sound::preset::Id presetId(
              {std::string(deviceName), engineIdx, std::string(presetName)});
          rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
       });

   onMusicDevicesDeviceAdded(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid,
              const ::capnzero::TextView& type,
              const ::capnzero::TextView& port,
              const ::capnzero::TextView& mediumId,
              ::capnzero::UInt8 midiVoiceOffset) {
                  ::capnzero::Data uuidData(uuid);
                  rSignals.MusicDevices__deviceAdded(uuidData, type, port, mediumId, midiVoiceOffset);
       });
   onMusicDevicesDeviceRemoved(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid) {

       });

   onSoundDevicesActualPresetChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
          const ::capnzero::TextView& presetName) {

       });

   onTransportControlEnabledChanged(
       [&rSignals](const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 enabled) {

       });
}