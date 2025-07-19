#include "TempoRpc.h"

#include "BeatTick.h"
#include "MusicDeviceHolder.h"

using namespace uiadapter::capnzero;

TempoRpc::TempoRpc(::capnzero::MidiEmRt::MidiEmRtServer::Signals& rSignals,
                   base::musicDevice::rt::DataHolder& rMdHolder) :
   m_rMdHolder(rMdHolder)
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

void TempoRpc::increaseRelativeTempo(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Float32 bpm)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMdHolder.musicDevices.find(uuid_);
   if (iter != m_rMdHolder.musicDevices.end())
   {
      auto midiOutEntry = m_rMdHolder.midiHolder.midiOutEntry(iter->second->mediumId());
      if(midiOutEntry)
      {
         midiOutEntry->offsetSpeedBpm = bpm;
      }
   }

}

void TempoRpc::setBpm(::capnzero::Float32 bpm)
{
   base::tempo::BeatTick::instance().setBpm(bpm);
}
