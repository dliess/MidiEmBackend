#ifndef UI_ADAPTER_CAPNZERO_RT_SERVER_H
#define UI_ADAPTER_CAPNZERO_RT_SERVER_H

#include "MidiEmRt_Server.h"

namespace base::instruments
{
struct Instruments;
}
namespace base::musicDevice
{
struct Holder;
}
namespace base::musicDevice
{
class TransportControl;
}
namespace base
{
class AbletonLinkWrapper;
}
namespace base::midifriends
{
class Router;
}
namespace base::musicDevice::controller
{
class EventRouter;
}
namespace base::musicDevice::sound
{
class ParameterSceneContainer;
}

namespace uiadapter::capnzero
{
class RtServer : public ::capnzero::MidiEmRt::MidiEmRtServer
{
public:
   RtServer(zmq::context_t& rZmqContext, const std::string& rpcBindAddr,
            const std::string& signalBindAddr,
            base::instruments::Instruments& rInstruments,
            base::musicDevice::Holder& rMusicDeviceHolder,
            base::musicDevice::TransportControl& rTransportControl,
            base::AbletonLinkWrapper& rAbletonLinkWrapper,
            base::midifriends::Router& rMidiRouter,
            base::musicDevice::controller::EventRouter& rCtrlEventRouter,
            base::musicDevice::sound::ParameterSceneContainer&
                rParameterSceneContainer);
   using Super    = ::capnzero::MidiEmRt::MidiEmRtServer;
   using WaitMode = Super::WaitMode;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_SERVER_H