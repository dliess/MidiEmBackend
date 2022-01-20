#include "AbletonLinkRpc.h"
#include "AbletonLinkWrapper.h"

using namespace uiadapter::capnzero;

AbletonLinkRpc::AbletonLinkRpc(base::AbletonLinkWrapper& rAbletonLinkWrapper) noexcept:
    m_rAbletonLinkWrapper(rAbletonLinkWrapper)
{
}

void AbletonLinkRpc::toggleEnabled()
{
    m_rAbletonLinkWrapper.enable(!m_rAbletonLinkWrapper.isEnabled());
}

void AbletonLinkRpc::toggleReactOnTransport()
{
    m_rAbletonLinkWrapper.reactOnTransport(!m_rAbletonLinkWrapper.reactsOnTransport());
}
