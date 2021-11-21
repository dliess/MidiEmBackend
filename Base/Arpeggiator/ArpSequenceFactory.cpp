#include "ArpSequenceFactory.h"

using namespace base::arp;

ArpSequenceFactory::ArpSequenceFactory(NoteContainer& rIncomingNoteBuffer,
                                       ArpSequence& rArpSequence) noexcept :
    m_rIncomingNoteBuffer(rIncomingNoteBuffer),
    m_rArpSequence(rArpSequence)
{
   m_rIncomingNoteBuffer.onChanged([this]() { m_dirty = true; });
}

void ArpSequenceFactory::createIfDirty() noexcept
{
   if(!m_dirty)
   {
      return;
   }
   m_rArpSequence.clear();
   switch(m_algorithm)
   {
      case Algorithm::Up:
      {
         for(const auto& e : m_rIncomingNoteBuffer)
         {

         }
         break;
      }
      case  Algorithm::Down:
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
