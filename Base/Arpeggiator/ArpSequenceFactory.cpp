#include "ArpSequenceFactory.h"

#include <map>

using namespace base::arp;

ArpSequenceFactory::ArpSequenceFactory(NoteContainer& rIncomingNoteBuffer,
                                       ArpSequence& rArpSequence) noexcept :
    m_rIncomingNoteBuffer(rIncomingNoteBuffer), m_rArpSequence(rArpSequence)
{
   m_rIncomingNoteBuffer.onChanged([this]() { m_dirty = true; });
}

void ArpSequenceFactory::createIfDirty() noexcept
{
   if (!m_dirty)
   {
      return;
   }
   m_rArpSequence.clear();
   std::byte stackBuf[1024];
   util::PrintAlloc oom("Out of Memory", std::pmr::null_memory_resource());
   std::pmr::monotonic_buffer_resource mbr(&oom);
   std::pmr::unsynchronized_pool_resource pool(&mbr);
   std::pmr::map<int, float> map(&pool);
   switch (m_algorithm)
   {
      case Algorithm::Up:
      {
         for (const auto& e : m_rIncomingNoteBuffer)
         {
            map.emplace(e.note, e.velocity);
         }
         int i = 0;
         for (auto it = map.begin(); it != map.end(); ++it)
         {
            m_rArpSequence.push_back(it->first, it->second);
         }
         break;
      }
      case Algorithm::Down:
      {
         break;
      }
      case Algorithm::UpDown:
      {
         break;
      }
      case Algorithm::Random:
      {
         break;
      }
      case Algorithm::RecvOrder:
      {
         break;
      }
      case Algorithm::CustomSequence:
      {
         break;
      }
   }
   m_dirty = false;
}

void ArpSequenceFactory::setRange(RangeType rangeType, int value) noexcept
{
   if (m_rangeType != rangeType)
   {
      m_rangeType = rangeType;
      m_dirty     = true;
      emitRangeTypeChanged(m_rangeType);
   }
   if (m_range != value)
   {
      m_range = value;
      m_dirty = true;
      emitRangeChanged(m_range);
   }
}

void ArpSequenceFactory::setAlgorithm(Algorithm algorithm) noexcept
{
   if (m_algorithm != algorithm)
   {
      m_algorithm = algorithm;
      m_dirty     = true;
      emitAlgorithmChanged(m_algorithm);
   }
}

size_t ArpSequenceFactory::rangeLen() const noexcept
{
   return m_rangeType == RangeType::Octave
              ? m_rIncomingNoteBuffer.size() * m_range
              : m_range;
}