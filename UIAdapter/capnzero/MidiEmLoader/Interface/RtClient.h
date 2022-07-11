#ifndef UI_ADAPTER_CAPNZERO_RT_CLIENT_H
#define UI_ADAPTER_CAPNZERO_RT_CLIENT_H

#include "LoaderServer.h"
#include "MidiEmRt_Client.h"
#include "LoaderServer.h"
#include "ControllerEventRouterLoader.h"

namespace base::musicDevice::factory
{
class Factory;
}

namespace uiadapter::capnzero
{
class RtClient : public ::capnzero::MidiEmRt::MidiEmRtClientRpc,
                 public ::capnzero::MidiEmRt::MidiEmRtClientSignals
{
public:
   RtClient(zmq::context_t& rZmqContext,
            const std::string& rtServerRpcAddr,
            const std::string& rtServerSignalAddr,
            LoaderServer::Signals& rSignals,
            base::musicDevice::factory::Factory& rMDFactory);
   using Super = ::capnzero::MidiEmRt::MidiEmRtClientSignals;
private:
   base::musicDevice::controller::loader::EventRoutes m_eventRoutes;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_CLIENT_H