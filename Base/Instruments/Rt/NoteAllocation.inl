#ifndef NOTEALLOCATION_INL
#define NOTEALLOCATION_INL

#include "NoteAllocation.h"

namespace base::instruments::rt
{

inline
NoteAllocation::NoteAllocation() noexcept
{
   m_noteAllocations.fill(FREE);
}

inline 
int NoteAllocation::incrementVoiceIndex(std::size_t maxSize) noexcept
{
   if(maxSize > 0)
   {
      ++m_currentVoiceIndex;
      m_currentVoiceIndex = m_currentVoiceIndex % maxSize;
   }
   return m_currentVoiceIndex;
}

inline 
int NoteAllocation::currentVoiceIndex() const noexcept
{
   return m_currentVoiceIndex;
}

inline
Ret<int> NoteAllocation::allocateVoice(int note, std::size_t numVoices) noexcept
{
   if(numVoices == 0)
   {
      return tl::unexpected(Error::invalidSize);
   }
   return safe_at(m_noteAllocations, note).map([numVoices, this](int* noteVoice) {
      if (*noteVoice == FREE)
      {
         *noteVoice = incrementVoiceIndex(numVoices);
      }
      return *noteVoice;
   });
}

inline
Ret<int> NoteAllocation::freeVoice(int note) noexcept
{
   return safe_at(m_noteAllocations, note).and_then([](int* noteVoice) -> Ret<int> {
      if (*noteVoice == FREE)
      {
         return tl::unexpected(Error::elementEmpty);
      }
      int tmp = *noteVoice;
      *noteVoice = FREE;
      return tmp;
   });
}

inline
Ret<int> NoteAllocation::getVoice(int note) noexcept
{
   return safe_at(m_noteAllocations, note).and_then([](int* noteVoice) -> Ret<int>{
      if (*noteVoice == FREE)
      {
         return tl::unexpected(Error::elementEmpty);
      }
      return *noteVoice;
   });
}   // namespace base::instruments::rt

} // namespace base::instruments::rt
#endif
