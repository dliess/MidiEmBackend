#include "BeatTick.h"
#include <loguru.hpp>

using namespace base::tempo;

BeatTick::BeatTick() noexcept:
   m_nextNotificationTimePoint(std::chrono::high_resolution_clock::now())
{}

void BeatTick::start() noexcept
{
   if(m_running) return;
   m_nextNotificationTimePoint = std::chrono::high_resolution_clock::now();
   m_running = true;
}

void BeatTick::stop() noexcept
{
   m_running = false;
}

void BeatTick::nextTimeSlot() noexcept
{
   if(!m_running) return;
   if(std::chrono::high_resolution_clock::now() >= m_nextNotificationTimePoint)
   {
     // LOG_F(INFO, "period {} ns", calcPeriodNs().count());
      m_nextNotificationTimePoint += calcPeriodNs();
      m_beatJiffies++;
   }
}

void BeatTick::incBpm(int cents) noexcept
{
   if(m_bpmCents + cents < 2 || m_bpmCents + cents > 40000) return;
   m_bpmCents.fetch_add(cents);
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

void BeatTick::setBpmCents(int cents) noexcept
{
   if(cents <= 0 || m_bpmCents == cents) return;
   m_bpmCents = cents;
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

void BeatTick::setNudgeCents(int cents) noexcept
{
   m_nudgeCents = cents;
   if(m_bpmChangeNotifCb) m_bpmChangeNotifCb(getBpmCents());
}

int BeatTick::getBpmCents() const noexcept
{
   return m_bpmCents + m_nudgeCents;
}

void BeatTick::registerBpmChangeNotifCb(std::function<void(int)> cb) noexcept
{
   m_bpmChangeNotifCb = cb;
}

std::chrono::nanoseconds BeatTick::getBeatPeriodNs() const noexcept
{
   return calcPeriodNs() * PPQ;
}

std::chrono::nanoseconds BeatTick::calcPeriodNs() const noexcept
{
   constexpr uint64_t NSEC_PER_MIN = std::chrono::nanoseconds(std::chrono::minutes(1)).count();
   return std::chrono::duration<int64_t, std::nano>( (NSEC_PER_MIN * 100) / (getBpmCents() * PPQ) );
}
