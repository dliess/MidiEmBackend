#ifndef BEAT_TICK_H
#define BEAT_TICK_H

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>

namespace base::tempo
{
class BeatTick
{
public:
   BeatTick(const BeatTick&) = delete;
   BeatTick& operator=(const BeatTick&) = delete;
   static inline BeatTick& instance() noexcept;
   static constexpr int PPQ = 240;
   uint64_t getBeatJiffies() const noexcept;
   void start() noexcept;
   void stop() noexcept;
   void nextTimeSlot() noexcept;
   void incBpm(int cents) noexcept;
   void setBpmCents(int cents) noexcept;
   void setNudgeCents(int cents) noexcept;
   int getBpmCents() const noexcept;
   void registerBpmChangeNotifCb(std::function<void(int)> cb) noexcept;
   void registerMidiClockNotifCb(std::function<void()> cb) noexcept;
   std::chrono::nanoseconds getBeatPeriodNs() const noexcept;

private:
   BeatTick() noexcept;
   uint64_t m_beatJiffies{0};
   std::chrono::nanoseconds calcPeriodNs() const noexcept;
   std::atomic<bool> m_running{false};
   std::atomic<int> m_bpmCents{12000};
   std::atomic<int> m_nudgeCents{0};
   std::chrono::system_clock::time_point m_nextNotificationTimePoint;
   std::function<void(int)> m_bpmChangeNotifCb;
};

} // namespace base::tempo

#include "BeatTick.inl"

#endif