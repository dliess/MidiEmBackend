#include "TransportControlRpc.h"

#include "TransportControl.h"

using namespace uiadapter::capnzero;

TransportControlRpc::TransportControlRpc(
    base::TransportControl& rTransportControl) :
    m_rTransportControl(rTransportControl)
{
}

void TransportControlRpc::toggleEnabled(const ::capnzero::SpanCL<16>& uuid)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   m_rTransportControl.toggleEnabled(uuid_);
}

void TransportControlRpc::setStarted(::capnzero::Bool start)
{
   if (start)
   {
      m_rTransportControl.start();
   }
   else
   {
      m_rTransportControl.stop();
   }
}

void TransportControlRpc::toggleStarted()
{
   m_rTransportControl.toggleStartStop();
}

void TransportControlRpc::setQuantizedStart(::capnzero::Bool val)
{
   m_rTransportControl.setStartOnBeat(val);
}

void TransportControlRpc::toggleQuantizedStart()
{
   m_rTransportControl.setStartOnBeat(!m_rTransportControl.getStartOnBeat());
}