#ifndef TEMPO_HANDLER_INL
#define TEMPO_HANDLER_INL

#include "BeatTick.h"

inline base::tempo::BeatTick& base::tempo::BeatTick::instance() noexcept
{
   static BeatTick theInstance;
   return theInstance;
}

inline double base::tempo::BeatTick::getPrevBeat() const noexcept
{
   return m_prevTickBeats ? m_prevTickBeats.value() : 0;
}

inline double base::tempo::BeatTick::getBeat() const noexcept
{
   return m_beat;
}

inline double base::tempo::BeatTick::getInBeatPos() const noexcept
{
   return m_beat - static_cast<long>(m_beat);
}

inline double base::tempo::BeatTick::getBpmNudged() const noexcept
{
   return m_bpm + m_nudge;
}

inline base::AbletonLinkWrapper& base::tempo::BeatTick::abletonLink() noexcept
{ 
   return m_abletonLink;
};

#endif
