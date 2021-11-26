#include "ArpSequencePlayer.h"

#include "BeatTick.h"
#include "loguru.hpp"

using namespace base::arp;

ArpSequencePlayer::ArpSequencePlayer(const ArpSequence& rArpSequence) noexcept :
    m_rArpSequence(rArpSequence)
{
}

void ArpSequencePlayer::start() noexcept
{
   LOG_F(INFO, "ArpSequencePlayer::start()");
   m_tStepEnd = base::tempo::BeatTick::instance().getBeatJiffies();
   m_tNoteOff = m_tStepEnd;
   m_actualIdx       = -1;
   m_started = true;
}

void ArpSequencePlayer::stop() noexcept
{
   LOG_F(INFO, "ArpSequencePlayer::stop()");
   if(m_actualIdx > -1)
   {
      const ArpSequence::NoteData noteToRelease = m_rArpSequence.get(m_actualIdx);
      //LOG_F(INFO, "ArpSequencePlayer::stop() NoteOff({})", noteToRelease.note);
      emitNoteOff(noteToRelease.note, 1.0);
   }
   m_released = true;
   m_actualIdx       = -1;
   m_started = false;
}

void ArpSequencePlayer::setGateFill(float gateFill) noexcept
{
   gateFill = std::min(gateFill, 1.0f);
   gateFill = std::max(gateFill, 0.0f);
   if (m_gateFill != gateFill)
   {
      m_gateFill = gateFill;
      emitGateFillChanged(m_gateFill);
   }
}

void ArpSequencePlayer::setStepLength(int stepLength) noexcept
{
   stepLength = std::max(stepLength, 1);
   if (m_stepLength != stepLength)
   {
      m_stepLength = stepLength;
      emitStepLengthChanged(m_stepLength);
   }
}

void ArpSequencePlayer::update()
{
   if(false == m_started || m_rArpSequence.size() == 0)
   {
      return;
   }
   const uint64_t now = base::tempo::BeatTick::instance().getBeatJiffies();
   if (now < m_tNoteOff)
   {
      return;
   }
   else
   {
      if (!m_released && m_actualIdx > -1)
      {
         const ArpSequence::NoteData noteToRelease =
             m_rArpSequence.get(m_actualIdx);
         emitNoteOff(noteToRelease.note, 1.0);
         m_released = true;
      }
      if (now >= m_tStepEnd)
      {
         do {
            m_tStepEnd += m_stepLength;
         } while (m_tStepEnd < now);
         const int noteOffTicks = std::min(int(m_stepLength * (1.0 - m_gateFill)), m_stepLength - 1);
         m_tNoteOff = m_tStepEnd - noteOffTicks;
         if (++m_actualIdx >= m_rArpSequence.size())
         {
            m_actualIdx = 0;
            emitTurnOver();
         }
         const ArpSequence::NoteData noteToActivate =
             m_rArpSequence.get(m_actualIdx);
         emitNoteOn(noteToActivate.note, noteToActivate.velocity);
         m_released = false;
      }
   }
}
