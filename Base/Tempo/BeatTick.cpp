#include "BeatTick.h"

#include <loguru.hpp>

using namespace base::tempo;

BeatTick::BeatTick() noexcept
{
   onBpmNudgedChanged([this](double bpm) {
      if (m_abletonLink.isEnabled())
      {
         m_abletonLink.setTempo(bpm);
      }
   }
   m_abletonLink.onEnabledChanged([this](bool enabled){
      if(enabled)
      {
         m_tLast = std::nullopt;
      }
   });
}

double BeatTick::nextTick() noexcept
{
   if (m_abletonLink.enabled())
   {
      const auto [bpm, timeNowUs, beat] = m_abletonLink.snapshot();
      m_beat                            = beat;
      setBpm(bpm);
   }
   else
   {
      if(!m_tLast)
      {
         m_tLast = std::chrono::high_resolution_clock::now();
      }
      else
      {
         const auto tNow     = std::chrono::high_resolution_clock::now();
         const auto deltaTUs = duration_cast<microseconds>(tNow - *m_tLast);
         m_tLast             = tNow;
         m_beat += (deltaTUs.count * m_bpm) / (60000000.0);
      }
   }
}

void BeatTick::incBpm(double increment) noexcept
{
   const auto reqBpm = m_bpm + increment;
   if (reqBpm < BpmMin || reqBpm > BpmMax)
      return;
   m_bpm = reqBpm;
   emitBpmNudgedChanged(getBpmNudged());
}

void BeatTick::setBpm(double reqBpm) noexcept
{
   if (reqBpm < BpmMin || reqBpm > BpmMax)
      return;
   m_bpm = reqBpm;
   emitBpmNudgedChanged(getBpmNudged());
}

void BeatTick::setNudge(double nudge) noexcept
{
   m_nudge = nudge;
   emitBpmNudgedChanged(getBpmNudged());
}