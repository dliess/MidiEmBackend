#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   const std::string& rtServerRpcAddr,
                   const std::string& rtServerSignalAddr,
                   LoaderServer::Signals& rSignals,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext,
                                            rtServerRpcAddr),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext,
                                                rtServerSignalAddr)
{
   onSoundDevicesPresetChanged(
       [this, &rMDFactory](const ::capnzero::TextView& deviceName,
                           ::capnzero::Int16 engineIdx,
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
                                ::capnzero::Int16 midiVoiceOffset) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().addUuid2MdId(
              uuidData, MusicDeviceId{std::string(type), std::string(port)});
       });
   onMusicDevicesDeviceRemoved(
       [&rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().removeEntryForUuid(uuidData);
       });

   onAbletonLinkEnabledChanged([](::capnzero::Bool val) {
      // TODO: save it to file and load
   });
   onAbletonLinkOffsetTimeUsChanged([](::capnzero::Int32 val) {
      // TODO: save it to file and load
   });
}