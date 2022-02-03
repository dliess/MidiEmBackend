#include "BeatTick.h"

#include <loguru.hpp>

using namespace base::tempo;

BeatTick::BeatTick() noexcept
{
   m_abletonLink.onEnabledChanged([this](bool enabled){
      if(enabled)
      {
         m_tLast = std::nullopt;
      }
   });
}

double BeatTick::nextTick() noexcept
{
   if (m_abletonLink.isEnabled())
   {
      const auto [bpm, beat] = m_abletonLink.snapshot();
      m_beat                            = beat;
      m_bpm = bpm - m_nudge;
      emitBpmNudgedChanged(getBpmNudged());
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
         const auto deltaTUs = std::chrono::duration_cast<std::chrono::microseconds>(tNow - *m_tLast);
         m_tLast             = tNow;
         m_beat += (deltaTUs.count() * m_bpm) / (60000000.0);
      }
   }
   if(!m_prevTickBeats)
   {
      m_prevTickBeats = m_beat;
   }
   const auto tmp = m_prevTickBeats.value();
   m_prevTickBeats = m_beat;
   return m_beat - tmp;
}

void BeatTick::incBpm(double increment) noexcept
{
   const auto reqBpm = m_bpm + increment;
   if (reqBpm < BpmMin || reqBpm > BpmMax)
      return;
   m_bpm = reqBpm;
   emitBpmNudgedChanged(getBpmNudged());
   if (m_abletonLink.isEnabled()) m_abletonLink.setTempo(getBpmNudged());
}

void BeatTick::setBpm(double reqBpm) noexcept
{
   if (reqBpm < BpmMin || reqBpm > BpmMax)
      return;
   m_bpm = reqBpm;
   emitBpmNudgedChanged(getBpmNudged());
   if (m_abletonLink.isEnabled()) m_abletonLink.setTempo(getBpmNudged());
}

void BeatTick::setNudge(double nudge) noexcept
{
   m_nudge = nudge;
   emitBpmNudgedChanged(getBpmNudged());
   if (m_abletonLink.isEnabled()) m_abletonLink.setTempo(getBpmNudged());
}