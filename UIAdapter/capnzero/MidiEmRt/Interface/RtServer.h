#ifndef UI_ADAPTER_CAPNZERO_RT_SERVER_H
#define UI_ADAPTER_CAPNZERO_RT_SERVER_H

#include "MidiEmRt_Server.h"

namespace base::instruments { struct Instruments; }
namespace base::musicDevice { class MusicDeviceContainer; }
namespace base::musicDevice { class TransportControl; }
namespace base { class AbletonLinkWrapper; }
namespace base::midifriends { class Router; }

namespace uiadapter::capnzero
{

class RtServer : public ::capnzero::MidiEmRt::MidiEmRtServer
{
public:
    RtServer(zmq::context_t& rZmqContext,
           base::instruments::Instruments& rInstruments,
           base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer,
           base::musicDevice::TransportControl& rTransportControl,
           base::AbletonLinkWrapper& rAbletonLinkWrapper,
           base::midifriends::Router& rMidiRouter);
    using Super = ::capnzero::MidiEmRt::MidiEmRtServer;
    using WaitMode = Super::WaitMode;
};

} // namespace uiadapter::capnzero

#endif // UI_ADAPTER_CAPNZERO_RT_SERVER_H