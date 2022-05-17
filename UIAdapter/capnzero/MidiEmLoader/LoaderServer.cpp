#include "LoaderServer.h"

#include "JsonCast.h"   // meta::serialize
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

LoaderServer::LoaderServer(zmq::context_t& rZmqContext,
                           base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(
        rZmqContext, "tcp://*:55557", "tcp://*:55558",
        std::make_unique<LoaderRpc>(signals(), rMDFactory))
{
}
