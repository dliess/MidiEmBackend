#include "BeatTick.h"

#include <loguru.hpp>
#include <limits>

using namespace base::tempo;

std::pair<double, std::chrono::microseconds> BeatTick::nextTick() noexcept
{
   std::chrono::microseconds deltaTUs{0};
   if (m_abletonLink.isEnabled())
   {
      const auto [bpm, beat, tNowUs] = m_abletonLink.snapshot();
      if(!m_lastAbletonLinkSampleTimePointUs)
      {
         m_lastAbletonLinkSampleTimePointUs = tNowUs;
         m_tLast = std::nullopt;
      }
      m_beat = beat;
      deltaTUs = tNowUs - *m_lastAbletonLinkSampleTimePointUs;
      m_lastAbletonLinkSampleTimePointUs = tNowUs;
      if(std::fabs(m_bpm - (bpm - m_nudge)) > std::numeric_limits<double>::epsilon())
      {
         m_bpm = bpm - m_nudge;
         emitBpmNudgedChanged(getBpmNudged());
      }
      m_abletonLink.offset.calcOffsets(deltaTUs);
      m_beat += m_abletonLink.offset.offsetBeats();
   }
   else
   {
      if(!m_tLast)
      {
         m_tLast = std::chrono::high_resolution_clock::now();
         m_lastAbletonLinkSampleTimePointUs = std::nullopt;
      }
      else
      {
         const auto tNow     = std::chrono::high_resolution_clock::now();
         deltaTUs = std::chrono::duration_cast<std::chrono::microseconds>(tNow - *m_tLast);
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
   return std::make_pair(m_beat - tmp, deltaTUs);
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