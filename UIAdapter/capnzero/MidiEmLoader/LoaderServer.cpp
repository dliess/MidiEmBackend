#include "LoaderServer.h"

#include "JsonCast.h"   // meta::serialize
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

LoaderServer::LoaderServer(zmq::context_t& rZmqContext,
                           const std::string& rpcBindAddr,
                           const std::string& signalBindAddr,
                           base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(
        rZmqContext, rpcBindAddr, signalBindAddr,
        std::make_unique<LoaderRpc>(signals(), rMDFactory))
{
}
