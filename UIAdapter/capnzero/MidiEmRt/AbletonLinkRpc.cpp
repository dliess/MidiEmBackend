#include "AbletonLinkRpc.h"
#include "AbletonLinkWrapper.h"

using namespace uiadapter::capnzero;

AbletonLinkRpc::AbletonLinkRpc(base::AbletonLinkWrapper& rAbletonLinkWrapper) noexcept:
    m_rAbletonLinkWrapper(rAbletonLinkWrapper)
{
}

void AbletonLinkRpc::setEnabled(::capnzero::Bool val)
{
    m_rAbletonLinkWrapper.enable(val);
}

void AbletonLinkRpc::toggleEnabled()
{
    m_rAbletonLinkWrapper.enable(!m_rAbletonLinkWrapper.isEnabled());
}

void AbletonLinkRpc::setReactOnTransport(::capnzero::Bool val)
{
    m_rAbletonLinkWrapper.reactOnTransport(val);
}

void AbletonLinkRpc::toggleReactOnTransport()
{
    m_rAbletonLinkWrapper.reactOnTransport(!m_rAbletonLinkWrapper.reactsOnTransport());
}

void AbletonLinkRpc::setOffsetSetupTempo(::capnzero::Float32 val)
{
    m_rAbletonLinkWrapper.offset.setOffsetSetupTempo(val);
}