#ifndef BASE_ARPEGGIATOR_H
#define BASE_ARPEGGIATOR_H

#include <memory>

#include "CallbackSignal.h"
#include "EnumReflect.h"

namespace base::arp
{
DECLARE_ENUM(Algorithm, uint, Up, Down, UpDown, Random, RecvOrder,
             CustomSequence);
DECLARE_ENUM(RangeType, uint, Octave, Notes);

struct ArpeggiatorPrivate;
class Arpeggiator
{
public:
   Arpeggiator();
   ~Arpeggiator();
   Arpeggiator(const Arpeggiator&) = delete;
   Arpeggiator(Arpeggiator&&)      = default;
   Arpeggiator& operator=(const Arpeggiator&) = delete;
   Arpeggiator& operator=(Arpeggiator&&) = default;

   void update() noexcept;
   void bypass(bool onOff) noexcept;
   void noteOn(int note, float velocity) noexcept;
   void noteOff(int note, float velocity) noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setGateFill(float gateFill) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;

   CB_SIGNAL_IF(BypassChanged, bool);
   CB_SIGNAL_IF(NoteOn, int, float);
   CB_SIGNAL_IF(NoteOff, int, float);
   CB_SIGNAL_IF(RangeTypeChanged, int);
   CB_SIGNAL_IF(RangeChanged, int);
   CB_SIGNAL_IF(GateFillChanged, float);
   CB_SIGNAL_IF(AlgorithmChanged, Algorithm);

private:
    std::unique_ptr<ArpeggiatorPrivate> m_pImpl;
};

}   // namespace base::arp

#endif