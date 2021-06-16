#ifndef TEMPO_RPC_H
#define TEMPO_RPC_H

#include "MidiEmTempoRpcIf.h"
#include "TapTempoHandler.h"
#include "MidiEm_Server.h"

namespace uiadapter::capnzero
{

class TempoRpc : public ::capnzero::MidiEm::TempoRpcIf
{
public:
    TempoRpc(::capnzero::MidiEm::MidiEmServer::Signals& rSignals);
	void start() override;
	void stop() override;
	void tapTempo() override;
	void setNudge(::capnzero::UInt32 bpmCents) override;
	void increaseBpm(::capnzero::UInt32 bpmCents) override;
	void setBpmCents(::capnzero::UInt32 val) override;
private:
    base::tempo::TapTempoHandler m_tapTempoHandler;
};

} // namespace uiadapter::capnzero
#endif