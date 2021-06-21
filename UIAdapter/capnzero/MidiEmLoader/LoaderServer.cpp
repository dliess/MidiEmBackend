#include "LoaderServer.h"
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

#include "JsonCast.h" // meta::serialize

using namespace uiadapter::capnzero;

LoaderServer::LoaderServer(zmq::context_t &rZmqContext, base::musicDevice::Factory& rMDFactory)  :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(rZmqContext, "tcp://*:5557",
                                           "tcp://*:5558",
                                           std::make_unique<LoaderRpc>()),
    m_rMDFactory(rMDFactory)
{
}
