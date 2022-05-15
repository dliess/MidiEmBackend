#ifndef ABLETON_LINK_RPC_H
#define ABLETON_LINK_RPC_H

#include "MidiEmRtAbletonLinkRpcIf.h"

namespace base { class AbletonLinkWrapper; }

namespace uiadapter::capnzero
{
class AbletonLinkRpc : public ::capnzero::MidiEmRt::AbletonLinkRpcIf
{
public:
    AbletonLinkRpc(base::AbletonLinkWrapper& rAbletonLinkWrapper) noexcept;
	void setEnabled(::capnzero::Bool val) override;
	void toggleEnabled() override;
    void setReactOnTransport(::capnzero::Bool val) override;
	void toggleReactOnTransport() override;
	void setOffsetSetupTempo(::capnzero::Float32 val) override;
private:
	base::AbletonLinkWrapper& m_rAbletonLinkWrapper;
};

}   // namespace uiadapter::capnzero
#endif   // ABLETON_LINK_RPC_H