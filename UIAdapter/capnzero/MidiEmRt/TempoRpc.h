#ifndef TEMPO_RPC_H
#define TEMPO_RPC_H

#include "MidiEmRtTempoRpcIf.h"
#include "MidiEmRt_Server.h"
#include "TapTempoHandler.h"

namespace base::musicDevice { struct Holder; }

namespace uiadapter::capnzero
{
class TempoRpc : public ::capnzero::MidiEmRt::TempoRpcIf
{
public:
   TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals,
            base::musicDevice::Holder& rMdHolder);
   void tapTempo() override;
   void setNudge(::capnzero::Float32 bpm) override;
   void increaseBpm(::capnzero::Float32 bpm) override;
   void increaseRelativeTempoOfAll(::capnzero::Float32 bpm) override;
   void increaseRelativeTempo(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::Float32 bpm) override;
   void setBpm(::capnzero::Float32 bpm) override;

private:
   base::tempo::TapTempoHandler m_tapTempoHandler;
   base::musicDevice::Holder& m_rMdHolder;
};

}   // namespace uiadapter::capnzero
#endif