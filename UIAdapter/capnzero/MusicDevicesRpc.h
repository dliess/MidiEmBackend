#ifndef MUSIC_DEVICES_RPC_H
#define MUSIC_DEVICES_RPC_H

#include "MidiEmMusicDevicesRpcIf.h"

namespace uiadapter::capnzero {

class MusicDevicesRpc : public ::capnzero::MidiEm::MusicDevicesRpcIf {
public:
  void triggerSendMusicDeviceList() override;
  void registerForParameterChange(const ::capnzero::TextView &deviceIdStr,
                                  ::capnzero::Int16 voiceIdx) override;
  ReturnUnregisterForParameterChange
  unregisterForParameterChange(const ::capnzero::TextView &deviceIdStr, ::capnzero::Int16 voiceIdx,
                               const ::capnzero::Span &instrumentUUID) override;
};

} // namespace uiadapter::capnzero

#endif // MUSIC_DEVICES_RPC_H