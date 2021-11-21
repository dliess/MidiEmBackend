#ifndef BASE_ARPEGGIATOR_H
#define BASE_ARPEGGIATOR_H

#include <memory>

#include "CallbackSignal.h"
#include "EnumReflect.h"
#include "NoteContainer"

namespace base::arp
{
DECLARE_ENUM(Algorithm, uint, Up, Down, UpDown, Random, RecvOrder,
             CustomSequence);
DECLARE_ENUM(RangeType, uint, Octave, Notes);

class Arpeggiator
{
public:
   Arpeggiator();
   Arpeggiator(const Arpeggiator&) = delete;
   Arpeggiator(Arpeggiator&&)      = default;
   Arpeggiator& operator=(const Arpeggiator&) = delete;
   Arpeggiator& operator=(Arpeggiator&&) = default;

   void update() noexcept;
   void bypass(bool onOff) noexcept;
   void noteOn(float velocity) noexcept;
   void noteOff(float velocity) noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setGateFill(float gateFill) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;

   CB_SIGNAL(BypassChanged, bool);
   CB_SIGNAL(NoteOn, float);
   CB_SIGNAL(NoteOff, float);
   CB_SIGNAL(RangeTypeChanged, int);
   CB_SIGNAL(RangeChanged, int);
   CB_SIGNAL(GateFillChanged, float);
   CB_SIGNAL(AlgorithmChanged, Algorithm);

private:
   bool m_bypass{true};
   RangeType m_rangeType{RangeType::Octave};
   int m_range{1};
   float m_gateFill{0.5};
   Algorithm m_algorithm{Algorithm::Up};
   NoteContainer m_incomingNoteBuffer;
};

}   // namespace base::arp

#endif