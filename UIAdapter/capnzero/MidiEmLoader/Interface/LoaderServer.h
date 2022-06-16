#ifndef UI_ADAPTER_CAPNZERO_LOADER_SERVER_H
#define UI_ADAPTER_CAPNZERO_LOADER_SERVER_H

#include "MidiEmLoader_Server.h"

namespace base::musicDevice::factory
{
class Factory;
}
namespace uiadapter::capnzero
{
class LoaderServer : public ::capnzero::MidiEmLoader::MidiEmLoaderServer
{
public:
   LoaderServer(zmq::context_t& rZmqContext,
               const std::string& rpcBindAddr,
               const std::string& signalBindAddr,
                base::musicDevice::factory::Factory& rMDFactory);
   using Super    = ::capnzero::MidiEmLoader::MidiEmLoaderServer;
   using WaitMode = Super::WaitMode;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_LOADER_SERVER_H