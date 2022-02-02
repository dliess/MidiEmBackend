#ifndef TEMPO_HANDLER_INL
#define TEMPO_HANDLER_INL

#include "BeatTick.h"

inline base::tempo::BeatTick& base::tempo::BeatTick::instance() noexcept
{
   static BeatTick theInstance;
   return theInstance;
}

inline double base::tempo::BeatTick::getBeat() const noexcept
{
   return m_beat;
}

inline double base::tempo::BeatTick::getBpmNudged() const noexcept
{
   return m_bpm + m_nudge;
}


inline AbletonLinkWrapper& base::tempo::BeatTick::abletonLink() noexcept
{ 
   return *m_pAbletonLink.get();
};

#endif
