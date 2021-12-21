#ifndef BEAT_TICK_H
#define BEAT_TICK_H

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include "CallbackSignal.h"

namespace base::tempo
{
class BeatTick
{
public:
   BeatTick(const BeatTick&) = delete;
   BeatTick& operator=(const BeatTick&) = delete;
   static inline BeatTick& instance() noexcept;
   static constexpr int PPQ = 240;
   inline uint64_t getBeatJiffies() const noexcept;
   inline void setBeatJiffies(uint64_t jiffies) noexcept;
   void start() noexcept;
   void stop() noexcept;
   bool running() const noexcept;
   void nextTimeSlot() noexcept;
   void incBpm(int cents) noexcept;
   void setBpmCents(int cents) noexcept;
   void setBpmCentsNudged(int cents) noexcept;
   void setNudgeCents(int cents) noexcept;
   int getBpmCentsNudged() const noexcept;
   std::chrono::nanoseconds getBeatPeriodNs() const noexcept;

   CB_SIGNAL(RunningChanged, bool);
   CB_SIGNAL(BpmNudgedChanged, int);

private:
   BeatTick() noexcept;
   uint64_t m_beatJiffies{0};
   std::chrono::nanoseconds calcPeriodNs() const noexcept;
   std::atomic<bool> m_running{false};
   std::atomic<int> m_bpmCents{12000};
   std::atomic<int> m_nudgeCents{0};
   std::chrono::time_point<std::chrono::high_resolution_clock> m_nextNotificationTimePoint;
};

} // namespace base::tempo

#include "BeatTick.inl"

#endif