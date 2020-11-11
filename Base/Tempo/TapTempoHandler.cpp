#include "TapTempoHandler.h"

#include "BeatTick.h"

void base::tempo::TapTempoHandler::tap() noexcept
{
   auto now           = std::chrono::high_resolution_clock::now();
   auto diff          = now - m_lastTapTimepoint;
   m_lastTapTimepoint = now;
   if (diff >
       BeatTick::instance().getBeatPeriodNs() * NUM_PERIODS_TO_RESET_FROM)
   {
      m_sum   = std::chrono::nanoseconds::zero();
      m_count = 0;
      return;
   }
   m_sum += diff;
   ++m_count;
   if (m_count >= NUM_TAPS_TO_SET_BPM)
   {
      constexpr uint64_t NSEC_PER_MIN =
         std::chrono::nanoseconds(std::chrono::minutes(1)).count();
      BeatTick::instance().setBpmCents((NSEC_PER_MIN * 100 * m_count) /
                                       m_sum.count());
   }
}