#include "TapTempoHandler.h"

#include "BeatTick.h"

void base::tempo::TapTempoHandler::tap() noexcept
{
   constexpr uint64_t USEC_PER_MIN =
       std::chrono::microseconds(std::chrono::minutes(1)).count();
   const auto now    = std::chrono::high_resolution_clock::now();
   const auto diffUs = std::chrono::duration_cast<std::chrono::microseconds>(
       now - m_lastTapTimepoint);
   m_lastTapTimepoint = now;

   if (diffUs.count() > ((USEC_PER_MIN / BeatTick::instance().getBpmNudged()) *
                 NUM_PERIODS_TO_RESET_FROM))
   {
      m_sumUs = std::chrono::microseconds::zero();
      m_count = 0;
      return;
   }
   m_sumUs += diffUs;
   ++m_count;
   if (m_count >= NUM_TAPS_TO_SET_BPM)
   {
      BeatTick::instance().setBpm((USEC_PER_MIN * m_count) / m_sumUs.count());
   }
}