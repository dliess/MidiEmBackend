#ifndef TRANSPORT_CONTROL_RPC_H
#define TRANSPORT_CONTROL_RPC_H

#include "MidiEmRtTransportControlRpcIf.h"

namespace base::musicDevice
{
class TransportControl;
}

namespace uiadapter::capnzero
{
class TransportControlRpc : public ::capnzero::MidiEmRt::TransportControlRpcIf
{
public:
   TransportControlRpc(base::musicDevice::TransportControl& rTransportControl);
   void toggleEnabled(const ::capnzero::SpanCL<16>& uuid) override;
	void setStarted(::capnzero::Bool start) override;
	void toggleStarted() override;
	void setQuantizedStart(::capnzero::Bool val) override;
   void toggleQuantizedStart() override;

private:
   base::musicDevice::TransportControl& m_rTransportControl;
};

}   // namespace uiadapter::capnzero
#endif