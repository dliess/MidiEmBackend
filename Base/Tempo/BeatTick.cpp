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
   emitRunningChanged(true);
}

void BeatTick::stop() noexcept
{
   if(!m_running) return;
   m_running = false;
   emitRunningChanged(false);
}

bool BeatTick::running() const noexcept
{
   return m_running;
}

void BeatTick::nextTimeSlot() noexcept
{
   if(!m_running) return;
   m_beatJiffiesBefore = m_beatJiffies;
   while(std::chrono::high_resolution_clock::now() > m_nextNotificationTimePoint)
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
   emitBpmNudgedChanged(getBpmCentsNudged());
}

void BeatTick::setBpmCents(int cents) noexcept
{
   if(cents <= 0 || m_bpmCents == cents) return;
   m_bpmCents = cents;
   emitBpmNudgedChanged(getBpmCentsNudged());
}

void BeatTick::setBpmCentsNudged(int cents) noexcept
{
   if(cents <= 0 || getBpmCentsNudged() == cents) return;
   m_bpmCents = cents - m_nudgeCents;
   emitBpmNudgedChanged(getBpmCentsNudged());
}

void BeatTick::setNudgeCents(int cents) noexcept
{
   if(m_nudgeCents != cents)
   {
      m_nudgeCents = cents;
      emitBpmNudgedChanged(getBpmCentsNudged());
   }
}

int BeatTick::getBpmCentsNudged() const noexcept
{
   return m_bpmCents + m_nudgeCents;
}

std::chrono::nanoseconds BeatTick::getBeatPeriodNs() const noexcept
{
   return calcPeriodNs() * PPQ;
}

std::chrono::nanoseconds BeatTick::calcPeriodNs() const noexcept
{
   constexpr uint64_t NSEC_PER_MIN = std::chrono::nanoseconds(std::chrono::minutes(1)).count();
   return std::chrono::duration<int64_t, std::nano>( (NSEC_PER_MIN * 100) / (getBpmCentsNudged() * PPQ) );
}
