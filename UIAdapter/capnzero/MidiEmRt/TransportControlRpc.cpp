#include "TransportControlRpc.h"

#include "TransportControl.h"

using namespace uiadapter::capnzero;

TransportControlRpc::TransportControlRpc(
    base::musicDevice::TransportControl& rTransportControl) :
    m_rTransportControl(rTransportControl)
{
}

void TransportControlRpc::toggleEnabled(const ::capnzero::SpanCL<16>& uuid)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   m_rTransportControl.toggleEnabled(uuid_);
}

void TransportControlRpc::setStarted(::capnzero::UInt8 val)
{
    if(val != 0)
    {
        m_rTransportControl.start();
    }
    else
    {
        m_rTransportControl.stop();
    }
}
