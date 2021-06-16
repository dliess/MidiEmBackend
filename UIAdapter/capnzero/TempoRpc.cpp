#include "TempoRpc.h"
#include "BeatTick.h"

using namespace uiadapter::capnzero;

TempoRpc::TempoRpc(::capnzero::MidiEm::MidiEmServer::Signals& rSignals)
{
	base::tempo::BeatTick::instance().registerRunningChangeNotifCb([&rSignals](bool running){
		rSignals.Tempo__beatTickStartedChanged(running);
	});
	base::tempo::BeatTick::instance().registerBpmChangeNotifCb([&rSignals](int bpmCents){
		rSignals.Tempo__bpmCentsChanged(bpmCents);
	});
	rSignals.registerTempoBeatTickStartedChangedSubscrCb([](::capnzero::MidiEm::MidiEmServer::Signals& rSignals){
		rSignals.Tempo__beatTickStartedChanged(base::tempo::BeatTick::instance().running());
	});
	rSignals.registerTempoBpmCentsChangedSubscrCb([](::capnzero::MidiEm::MidiEmServer::Signals& rSignals){
		rSignals.Tempo__bpmCentsChanged(base::tempo::BeatTick::instance().getBpmCents());
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

void TempoRpc::setNudge(::capnzero::UInt32 bpmCents)
{
	base::tempo::BeatTick::instance().setNudgeCents(bpmCents);
}

void TempoRpc::increaseBpm(::capnzero::UInt32 bpmCents)
{
	base::tempo::BeatTick::instance().incBpm(bpmCents);
}

void TempoRpc::setBpmCents(::capnzero::UInt32 val)
{
	base::tempo::BeatTick::instance().setBpmCents(val);
}
