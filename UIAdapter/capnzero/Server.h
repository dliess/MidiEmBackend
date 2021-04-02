#ifndef UI_ADAPTER_CAPNZERO_SERVER_H
#define UI_ADAPTER_CAPNZERO_SERVER_H

#include "MidiEm_Server.h"

namespace uiadapter::capnzero
{

class Server
{
public:
    Server();
private:
    zmq::context_t m_zmqContext;
    ::capnzero::MidiEm::MidiEmServer m_server;
};

} // namespace uiadapter::capnzero

#endif // UI_ADAPTER_CAPNZERO_SERVER_H