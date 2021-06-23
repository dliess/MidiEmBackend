#ifndef SOUND_DEVICES_RPC_H
#define SOUND_DEVICES_RPC_H

#include "MidiEmRtSoundDevicesRpcIf.h"

namespace base::musicDevice {
  class MusicDeviceContainer;
}

namespace uiadapter::capnzero {

class SoundDevicesRpc : public ::capnzero::MidiEmRt::SoundDevicesRpcIf {
public:
  SoundDevicesRpc(base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer);
	void registerForParameterChange(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx) override;
	void unregisterForParameterChange(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx) override;
	void incrementParameterValue(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceId, ::capnzero::Int16 paramIdx, ::capnzero::Float32 increment) override;
	void setParameterValue(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceId, ::capnzero::Int16 paramIdx, ::capnzero::Float32 value) override;
	void increaseMidiVoiceOffset(const ::capnzero::SpanCL<16>& uuid) override;
	void decreaseMidiVoiceOffset(const ::capnzero::SpanCL<16>& uuid) override;

private:
  base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
};

} // namespace uiadapter::capnzero

#endif // SOUND_DEVICES_RPC_H