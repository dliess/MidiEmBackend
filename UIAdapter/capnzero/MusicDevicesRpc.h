#ifndef MUSIC_DEVICES_RPC_H
#define MUSIC_DEVICES_RPC_H

#include "MidiEmMusicDevicesRpcIf.h"

namespace base::musicDevice {
  class MusicDeviceContainer;
}

namespace uiadapter::capnzero {

class MusicDevicesRpc : public ::capnzero::MidiEm::MusicDevicesRpcIf {
public:
  MusicDevicesRpc(base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer);
  void noteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
	void noteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
private:
  base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
};

} // namespace uiadapter::capnzero

#endif // MUSIC_DEVICES_RPC_H