#ifndef UI_ADAPTER_CAPNZERO_RT_CLIENT_H
#define UI_ADAPTER_CAPNZERO_RT_CLIENT_H

#include "LoaderServer.h"
#include "MidiEmRt_Client.h"
#include "LoaderServer.h"

// clang-format off
namespace base::musicDevice::factory { class Factory; }
namespace base::instruments { class Instruments; }
// clang-format on
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
            base::musicDevice::factory::Factory& rMDFactory,
            base::instruments::Instruments& rInstruments);
   using Super = ::capnzero::MidiEmRt::MidiEmRtClientSignals;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_CLIENT_H