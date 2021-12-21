#include "TempoRpc.h"
#include "BeatTick.h"

using namespace uiadapter::capnzero;

TempoRpc::TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals)
{
	base::tempo::BeatTick::instance().onRunningChanged([&rSignals](bool running){
		rSignals.Tempo__beatTickStartedChanged(running);
	});
	base::tempo::BeatTick::instance().onBpmNudgedChanged([&rSignals](int bpmCents){
		rSignals.Tempo__bpmCentsChanged(bpmCents);
	});
}

void TempoRpc::start()
{
	base::tempo::BeatTick::instance().start();
}

void TempoRpc::stop()
{
	base::tempo::BeatTick::instance().stop();
}

void TempoRpc::tapTempo()
{
	m_tapTempoHandler.tap();
}

void TempoRpc::setNudge(::capnzero::Int32 bpmCents)
{
	base::tempo::BeatTick::instance().setNudgeCents(bpmCents);
}

void TempoRpc::increaseBpm(::capnzero::Int32 bpmCents)
{
	base::tempo::BeatTick::instance().incBpm(bpmCents);
}

void TempoRpc::setBpmCents(::capnzero::UInt32 val)
{
	base::tempo::BeatTick::instance().setBpmCents(val);
}
