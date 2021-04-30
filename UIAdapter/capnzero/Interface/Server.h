#ifndef UI_ADAPTER_CAPNZERO_SERVER_H
#define UI_ADAPTER_CAPNZERO_SERVER_H

#include "MidiEm_Server.h"

namespace base::instruments { struct Instruments; }

namespace uiadapter::capnzero
{

class Server : public ::capnzero::MidiEm::MidiEmServer
{
public:
    Server(base::instruments::Instruments& rInstruments);
    using Super = ::capnzero::MidiEm::MidiEmServer;
    using WaitMode = Super::WaitMode;
private:
    zmq::context_t m_zmqContext;
};

} // namespace uiadapter::capnzero

#endif // UI_ADAPTER_CAPNZERO_SERVER_H