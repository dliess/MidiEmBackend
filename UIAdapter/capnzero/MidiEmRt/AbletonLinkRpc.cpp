#include "AbletonLinkRpc.h"
#include "AbletonLinkWrapper.h"

using namespace uiadapter::capnzero;

AbletonLinkRpc::AbletonLinkRpc(base::AbletonLinkWrapper& rAbletonLinkWrapper) noexcept:
    m_rAbletonLinkWrapper(rAbletonLinkWrapper)
{
}

void AbletonLinkRpc::setEnabled(::capnzero::UInt8 val)
{
    m_rAbletonLinkWrapper.enable(static_cast<bool>(val));
}

void AbletonLinkRpc::toggleEnabled()
{
    m_rAbletonLinkWrapper.enable(!m_rAbletonLinkWrapper.isEnabled());
}

void AbletonLinkRpc::setReactOnTransport(::capnzero::UInt8 val)
{
    m_rAbletonLinkWrapper.reactOnTransport(static_cast<bool>(val));
}

void AbletonLinkRpc::toggleReactOnTransport()
{
    m_rAbletonLinkWrapper.reactOnTransport(!m_rAbletonLinkWrapper.reactsOnTransport());
}
