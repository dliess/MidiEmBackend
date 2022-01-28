#ifndef TEMPO_RPC_H
#define TEMPO_RPC_H

#include "MidiEmRtTempoRpcIf.h"
#include "MidiEmRt_Server.h"
#include "TapTempoHandler.h"

namespace uiadapter::capnzero
{
class TempoRpc : public ::capnzero::MidiEmRt::TempoRpcIf
{
public:
   TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals);
   void start() override;
   void stop() override;
   void tapTempo() override;
   void setNudge(::capnzero::Int32 bpmCents) override;
   void increaseBpm(::capnzero::Int32 bpmCents) override;
   void increaseRelativeTempoOfAll(::capnzero::Int32 bpmCents) override;
   void increaseRelativeTempo(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::Int32 bpmCents) override;
   void setBpmCents(::capnzero::UInt32 val) override;

private:
   base::tempo::TapTempoHandler m_tapTempoHandler;
};

}   // namespace uiadapter::capnzero
#endif