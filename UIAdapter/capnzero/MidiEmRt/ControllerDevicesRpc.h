#ifndef CONTROLLER_DEVICES_RPC_H
#define CONTROLLER_DEVICES_RPC_H

#include "MidiEmRtControllerDevicesRpcIf.h"

namespace uiadapter::capnzero
{
class ControllerDevicesRpc : public ::capnzero::MidiEmRt::ControllerDevicesRpcIf
{
public:
   void setKeyboardMode(::capnzero::MidiEmRt::ControllerKeyboardMode mode) override;
};

}   // namespace uiadapter::capnzero
#endif