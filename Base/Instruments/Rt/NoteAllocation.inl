#ifndef NOTEALLOCATION_INL
#define NOTEALLOCATION_INL

namespace base::instruments::rt
{


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

inline int NoteAllocation::currentVoiceIndex() const noexcept
{
   return m_currentVoiceIndex;
}

inline
Ret<int> NoteAllocation::allocateVoice(int note, std::size_t numVoices) noexcept
{
   if(numVoices == 0)
   {
      return tl::unexpected(Error::numVoicesIsZero);
   }
   return safe_at(m_noteAllocations, note).map([numVoices, this](int* noteVoice) {
      if (*noteVoice == RtData::FREE)
      {
         *noteVoice = incrementVoiceIndex(numVoices);
         return *noteVoice;
      }
   });
}

} // namespace base::instruments::rt
#endif
