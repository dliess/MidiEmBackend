#ifndef BASE_ARP_SEQUENCE_FACTORY_H
#define BASE_ARP_SEQUENCE_FACTORY_H

#include "ArpSequence.h"
#include "ArpTypes.h"
#include "BaseSequence.h"
#include "CallbackSignal.h"
#include "NoteContainer.h"

namespace base::arp
{
class ArpSequenceFactory
{
public:
   ArpSequenceFactory(std::pmr::unsynchronized_pool_resource& pool,
                      NoteContainer& rIncomingNoteBuffer,
                      BaseSequence& rSequenceSource,
                      ArpSequence& rArpSequence) noexcept;
   void createIfDirty() noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;
   void updateSequence() noexcept;

   [[nodiscard]] Algorithm getAlgorithm() const noexcept
   {
      return m_algorithm;
   };
   [[nodiscard]] RangeType getRangeType() const noexcept { return m_rangeType; }
   [[nodiscard]] int getRange() const noexcept { return m_range; }

   CB_SIGNAL(AlgorithmChanged, Algorithm);
   CB_SIGNAL(RangeTypeChanged, RangeType);
   CB_SIGNAL(RangeChanged, int);

private:
   std::pmr::unsynchronized_pool_resource& m_rPool;
   NoteContainer& m_rIncomingNoteBuffer;
   BaseSequence& m_rSequenceSource;
   ArpSequence& m_rArpSequence;
   bool m_dirty{true};
   Algorithm m_algorithm{Algorithm::Up};
   RangeType m_rangeType{RangeType::Octave};
   int m_range{1};
   using NoteData = ArpSequence::NoteData;
   std::pmr::list<NoteData> m_actualBaseSequence;
   size_t rangeLen() const noexcept;
   static int keepInNoteRange(int note) noexcept;
   void expandToArpSequence(int offsetNote, float velocity) noexcept;
};

}   // namespace base::arp

#endif