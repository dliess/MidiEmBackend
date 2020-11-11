#ifndef TEMPO_HANDLER_INL
#define TEMPO_HANDLER_INL

#include "BeatTick.h"

inline base::tempo::BeatTick& base::tempo::BeatTick::instance() noexcept
{
   static BeatTick theInstance;
   return theInstance;
}

inline uint64_t base::tempo::BeatTick::getBeatJiffies() const noexcept
{
   return m_beatJiffies;
}

#endif
