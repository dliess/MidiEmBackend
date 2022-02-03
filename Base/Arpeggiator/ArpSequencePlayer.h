#ifndef BASE_ARP_SEQUENCE_PLAYER_H
#define BASE_ARP_SEQUENCE_PLAYER_H

#include <cstddef>
#include <memory_resource>
#include <list>

#include "ArpSequence.h"
#include "print_alloc.h"
#include "CallbackSignal.h"

namespace base::arp
{
class ArpSequencePlayer
{
public:
   ArpSequencePlayer(const ArpSequence& rArpSequence) noexcept;
   void start() noexcept;
   void stop() noexcept;
   void setGateFill(float gateFill) noexcept;
   void setStepLength(float beats) noexcept;
   void update();

   [[nodiscard]] float getGateFill() const noexcept { return m_gateFill; }
   [[nodiscard]] float getStepLength() const noexcept { return m_stepLength; }

   CB_SIGNAL(NoteOn, int, float);
   CB_SIGNAL(NoteOff, int, float);
   CB_SIGNAL(GateFillChanged, float);
   CB_SIGNAL(StepLengthChanged, float);
   CB_SIGNAL(TurnOver);
private:
   const ArpSequence& m_rArpSequence;
   bool m_started{false};
   float m_gateFill{0.5};
   double m_stepLength{1.0};
   int m_actualIdx{-1};
   double m_tStepEnd{0};
   double m_tNoteOff{0};
   bool m_released{true};
};

}   // namespace base::arp

#endif