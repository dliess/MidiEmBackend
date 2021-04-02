#include "Server.h"
#include "InstrumentsRpc.h"
#include "MusicDevicesRpc.h"

using namespace uiadapter::capnzero;

Server::Server()
    : m_server(m_zmqContext, "tcp://*:5555", "tcp://*:5556",
               std::make_unique<InstrumentsRpc>(),
               std::make_unique<MusicDevicesRpc>()) {}
