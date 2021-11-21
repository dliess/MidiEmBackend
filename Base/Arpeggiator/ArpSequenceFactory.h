#ifndef BASE_ARP_SEQUENCE_FACTORY_H
#define BASE_ARP_SEQUENCE_FACTORY_H

#include "CallbackSignal.h"
#include "NoteContainer.h"
#include "ArpTypes.h"

namespace base::arp
{
class ArpSequenceFactory
{
public:
   ArpSequenceFactory(NoteContainer& rIncomingNoteBuffer) noexcept;
   void createIfDirty() noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;

   CB_SIGNAL(AlgorithmChanged, Algorithm);
   CB_SIGNAL(RangeTypeChanged, RangeType);
   CB_SIGNAL(RangeChanged, int);

private:
   NoteContainer& m_rIncomingNoteBuffer;
   bool m_dirty{true};
   Algorithm m_algorithm{Algorithm::Up};
   RangeType m_rangeType{RangeType::Octave};
   int m_range{1};
};

}   // namespace base::arp

#endif