#ifndef TEMPO_RPC_H
#define TEMPO_RPC_H

#include "MidiEmRtTempoRpcIf.h"
#include "MidiEmRt_Server.h"
#include "TapTempoHandler.h"

namespace base::musicDevice::rt { struct DataHolder; }

namespace uiadapter::capnzero
{
class TempoRpc : public ::capnzero::MidiEmRt::TempoRpcIf
{
public:
   TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals,
            base::musicDevice::rt::DataHolder& rMdHolder);
   void tapTempo() override;
   void setNudge(::capnzero::Float32 bpm) override;
   void increaseBpm(::capnzero::Float32 bpm) override;
   void increaseRelativeTempo(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::Float32 bpm) override;
   void setBpm(::capnzero::Float32 bpm) override;

private:
   base::tempo::TapTempoHandler m_tapTempoHandler;
   base::musicDevice::rt::DataHolder& m_rMdHolder;
};

}   // namespace uiadapter::capnzero
#endif
