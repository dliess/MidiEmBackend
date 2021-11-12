#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext, "tcp://localhost:5555"),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext, "tcp://localhost:5556")
{
   onSoundDevicesPresetChanged([this, &rMDFactory](const ::capnzero::TextView& deviceName,
                                      ::capnzero::Int8 engineIdx,
                                      const ::capnzero::TextView& presetName) {
      const sound::preset::Id presetId(
          {std::string(deviceName), engineIdx, std::string(presetName)});
      rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
   });


}