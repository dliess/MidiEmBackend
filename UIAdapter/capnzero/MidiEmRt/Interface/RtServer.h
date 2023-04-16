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
namespace base
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
namespace base::musicDevice::sound
{
class ParameterSceneContainer;
}

namespace base::session
{
struct Tracks;
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
            base::TransportControl& rTransportControl,
            base::AbletonLinkWrapper& rAbletonLinkWrapper,
            base::midifriends::Router& rMidiRouter,
            base::musicDevice::sound::ParameterSceneContainer&
                rParameterSceneContainer,
            base::session::Tracks& rTracks);
   using Super    = ::capnzero::MidiEmRt::MidiEmRtServer;
   using WaitMode = Super::WaitMode;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_SERVER_H