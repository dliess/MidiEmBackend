#ifndef UI_ADAPTER_CAPNZERO_LOADER_SERVER_H
#define UI_ADAPTER_CAPNZERO_LOADER_SERVER_H

#include "MidiEmLoader_Server.h"

namespace uiadapter::capnzero
{

class LoaderServer : public ::capnzero::MidiEmLoader::MidiEmLoaderServer
{
public:
    LoaderServer(zmq::context_t& rZmqContext);
    using Super = ::capnzero::MidiEmLoader::MidiEmLoaderServer;
    using WaitMode = Super::WaitMode;
};

} // namespace uiadapter::capnzero

#endif // UI_ADAPTER_CAPNZERO_LOADER_SERVER_H