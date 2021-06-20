#ifndef UI_ADAPTER_CAPNZERO_SERVER_H
#define UI_ADAPTER_CAPNZERO_SERVER_H

#include "MidiEmRt_Server.h"

namespace base::instruments { struct Instruments; }
namespace base::musicDevice { class MusicDeviceContainer; }

namespace uiadapter::capnzero
{

class Server : public ::capnzero::MidiEmRt::MidiEmRtServer
{
public:
    Server(zmq::context_t& rZmqContext,
           base::instruments::Instruments& rInstruments,
           base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer);
    using Super = ::capnzero::MidiEmRt::MidiEmRtServer;
    using WaitMode = Super::WaitMode;
};

} // namespace uiadapter::capnzero

#endif // UI_ADAPTER_CAPNZERO_SERVER_H