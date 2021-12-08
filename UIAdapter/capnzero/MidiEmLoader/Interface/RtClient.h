#ifndef UI_ADAPTER_CAPNZERO_RT_CLIENT_H
#define UI_ADAPTER_CAPNZERO_RT_CLIENT_H

#include "LoaderServer.h"
#include "MidiEmRt_Client.h"
#include "LoaderServer.h"
#include "Settings.h"

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
            LoaderServer::Signals& rSignals,
            base::musicDevice::factory::Factory& rMDFactory);
   using Super = ::capnzero::MidiEmRt::MidiEmRtClientSignals;
private:
   util::Settings m_settings;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_CLIENT_H