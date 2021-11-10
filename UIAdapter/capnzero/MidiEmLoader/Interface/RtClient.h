#ifndef UI_ADAPTER_CAPNZERO_RT_CLIENT_H
#define UI_ADAPTER_CAPNZERO_RT_CLIENT_H

#include "LoaderServer.h"
#include "MidiEmRt_Client.h"

namespace base::musicDevice
{
class Factory;
}

namespace uiadapter::capnzero
{
class RtClient : public ::capnzero::MidiEmRt::MidiEmRtClientSignals
{
public:
   RtClient(zmq::context_t& rZmqContext, LoaderServer::Signals& rServerSignals,
            base::musicDevice::Factory& rMDFactory);
   using Super = ::capnzero::MidiEmRt::MidiEmRtClientSignals;

private:
   LoaderServer::Signals& m_rServerSignals;
   base::musicDevice::Factory& m_rMDFactory;
};

}   // namespace uiadapter::capnzero

#endif   // UI_ADAPTER_CAPNZERO_RT_CLIENT_H