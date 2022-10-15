#ifndef BEAT_TICK_H
#define BEAT_TICK_H

#include <chrono>
#include <optional>

#include "AbletonLinkWrapper.h"
#include "CallbackSignal.h"
namespace base::tempo
{
class BeatTick
{
public:
   BeatTick(const BeatTick&) = delete;
   BeatTick& operator=(const BeatTick&) = delete;
   BeatTick(BeatTick&&) noexcept        = delete;
   BeatTick& operator=(BeatTick&&) noexcept = delete;
   static inline BeatTick& instance() noexcept;
   inline double getPrevBeat() const noexcept;
   inline double getBeat() const noexcept;
   inline double getInBeatPos() const noexcept;
   std::pair<double, std::chrono::microseconds> nextTick() noexcept;
   void incBpm(double increment) noexcept;
   void setBpm(double value) noexcept;
   void setNudge(double nudge) noexcept;
   double getBpmNudged() const noexcept;
   inline AbletonLinkWrapper& abletonLink() noexcept;

   CB_SIGNAL(BpmNudgedChanged, double);

private:
   BeatTick() = default;
   double m_beat{0.0};
   double m_bpm{120.0};
   double m_nudge{0.0};
   std::optional<std::chrono::microseconds> m_lastAbletonLinkSampleTimePointUs;
   std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>
       m_tLast;
   std::optional<double> m_prevTickBeats;
   AbletonLinkWrapper m_abletonLink;
   static constexpr double BpmMin{0.02};
   static constexpr double BpmMax{400.0};
};

}   // namespace base::tempo

#include "BeatTick.inl"

#endif