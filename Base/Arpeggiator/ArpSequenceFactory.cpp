#include "ArpSequenceFactory.h"

#include <map>
#include <random>

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
   static constexpr int NOTES_IN_OCTAVE = 12;
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
         auto it = map.begin();
         for (int i = 0; i < rangeLen(); ++i)
         {
            m_rArpSequence.push_back(it->first + ((i / map.size()) * NOTES_IN_OCTAVE),
                                     it->second);
            if (++it == map.end())
            {
               it = map.begin();
            }
         }
         break;
      }
      case Algorithm::Down:
      {
         for (const auto& e : m_rIncomingNoteBuffer)
         {
            map.emplace(e.note, e.velocity);
         }
         auto it = map.rbegin();
         for (int i = 0; i < rangeLen(); ++i)
         {
            m_rArpSequence.push_back(it->first + ((i / map.size()) * NOTES_IN_OCTAVE),
                                     it->second);
            if (++it == map.rend())
            {
               it = map.rbegin();
            }
         }
         break;
      }
      case Algorithm::UpDown:
      {
         for (const auto& e : m_rIncomingNoteBuffer)
         {
            map.emplace(e.note, e.velocity);
         }
         bool up = true;
         auto it = map.begin();
         for (int i = 0; i < rangeLen(); ++i)
         {
            m_rArpSequence.push_back(it->first + ((i / (2 * map.size() - 1)) * NOTES_IN_OCTAVE),
                                     it->second);
            if(up)
            {
               ++it;
               if (it == map.end())
               {
                  up = false;
                  --it;
               }
            }
            else
            {
               if (it == map.begin())
               {
                  up = true;
               }
               else
               {
                  --it;
               }
            }
         }
         break;
      }
      case Algorithm::Random:
      {
         std::random_device r;
         std::default_random_engine e1(r());
         std::uniform_real_distribution<float> uniform_dist(0.0, m_rIncomingNoteBuffer.size() - 0.1);
         for (int i = 0; i < rangeLen(); ++i)
         {
            auto it = m_rIncomingNoteBuffer.begin();
            std::advance(it, int(uniform_dist(e1)));
            m_rArpSequence.push_back(it->note + ((i / m_rIncomingNoteBuffer.size()) * NOTES_IN_OCTAVE),
                                     it->velocity);
         }
         break;
      }
      case Algorithm::RecvOrder:
      {
         auto it = m_rIncomingNoteBuffer.begin();
         for (int i = 0; i < rangeLen(); ++i)
         {
            m_rArpSequence.push_back(it->note + ((i / map.size()) * NOTES_IN_OCTAVE), it->velocity);
            if (++it == m_rIncomingNoteBuffer.end())
            {
               it = m_rIncomingNoteBuffer.begin();
            }
         }
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
   value = std::max(value, m_rangeType == RangeType::Octave ? 0 : 1);
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