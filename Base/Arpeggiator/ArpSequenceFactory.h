#ifndef BASE_ARP_SEQUENCE_FACTORY_H
#define BASE_ARP_SEQUENCE_FACTORY_H

#include "ArpTypes.h"
#include "CallbackSignal.h"
#include "NoteContainer.h"
#include "ArpSequence.h"

namespace base::arp
{
class ArpSequenceFactory
{
public:
   ArpSequenceFactory(NoteContainer& rIncomingNoteBuffer,
                      ArpSequence& rArpSequence) noexcept;
   void createIfDirty() noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;

   [[nodiscard]] Algorithm getAlgorithm() const noexcept { return m_algorithm; };
   [[nodiscard]] RangeType getRangeType() const noexcept { return m_rangeType; }
   [[nodiscard]] int getRange() const noexcept { return m_range; }

   CB_SIGNAL(AlgorithmChanged, Algorithm);
   CB_SIGNAL(RangeTypeChanged, RangeType);
   CB_SIGNAL(RangeChanged, int);

private:
   NoteContainer& m_rIncomingNoteBuffer;
   ArpSequence& m_rArpSequence;
   bool m_dirty{true};
   Algorithm m_algorithm{Algorithm::Up};
   RangeType m_rangeType{RangeType::Octave};
   int m_range{1};
   size_t rangeLen() const noexcept;
};

}   // namespace base::arp

#endif