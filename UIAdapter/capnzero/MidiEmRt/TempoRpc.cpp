#include "TempoRpc.h"

#include "BeatTick.h"

using namespace uiadapter::capnzero;

TempoRpc::TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals)
{
   base::tempo::BeatTick::instance().onBpmNudgedChanged(
       [&rSignals](double bpm) { rSignals.Tempo__bpmChanged(bpm); });
}

void TempoRpc::tapTempo() { m_tapTempoHandler.tap(); }

void TempoRpc::setNudge(::capnzero::Float32 bpm)
{
   base::tempo::BeatTick::instance().setNudge(bpm);
}

void TempoRpc::increaseBpm(::capnzero::Float32 bpm)
{
   base::tempo::BeatTick::instance().incBpm(bpm);
}

void TempoRpc::increaseRelativeTempoOfAll(::capnzero::Float32 bpm) {}

void TempoRpc::increaseRelativeTempo(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Float32 bpm)
{
}

void TempoRpc::setBpm(::capnzero::Float32 bpm)
{
   base::tempo::BeatTick::instance().setBpm(bpm);
}
