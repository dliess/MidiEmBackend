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
   void setStepLength(int beatTicks) noexcept;
   void update();

   CB_SIGNAL(NoteOn, int, float);
   CB_SIGNAL(NoteOff, int, float);
   CB_SIGNAL(GateFillChanged, float);
   CB_SIGNAL(StepLengthChanged, int);
   CB_SIGNAL(TurnOver);
private:
   const ArpSequence& m_rArpSequence;
   float m_gateFill{0.5};
   int m_stepLength{240};
   int m_actualIdx{0};
   uint64_t m_tStepEnd{0};
   uint64_t m_tNoteOff{0};
   bool m_released{true};
};

}   // namespace base::arp

#endif