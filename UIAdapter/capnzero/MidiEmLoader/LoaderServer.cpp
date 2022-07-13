#include "LoaderServer.h"

#include "JsonCast.h"   // meta::serialize
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"
#include "ControllerEventRouterLoader.h"
#include "LdControllerEventRouterRpc.h"

using namespace uiadapter::capnzero;

LoaderServer::LoaderServer(zmq::context_t& rZmqContext,
                           const std::string& rpcBindAddr,
                           const std::string& signalBindAddr,
                           base::musicDevice::factory::Factory& rMDFactory,
                           base::musicDevice::controller::loader::EventRoutes& rEventRoutes) :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(
        rZmqContext, rpcBindAddr, signalBindAddr,
        std::make_unique<LoaderRpc>(signals(), rMDFactory),
        std::make_unique<LdControllerEventRouterRpc>(rMDFactory, rEventRoutes))
{
}
