#include "MusicDevicesRpc.h"

using namespace uiadapter::capnzero;

void MusicDevicesRpc::triggerSendMusicDeviceList() {}

void MusicDevicesRpc::registerForParameterChange(
    const ::capnzero::TextView &deviceIdStr, ::capnzero::Int16 voiceIdx) {}
MusicDevicesRpc::ReturnUnregisterForParameterChange
MusicDevicesRpc::unregisterForParameterChange(
    const ::capnzero::TextView &deviceIdStr, ::capnzero::Int16 voiceIdx,
    const ::capnzero::Span &instrumentUUID) {}
