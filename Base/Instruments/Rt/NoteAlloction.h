#ifndef NOTE_ALLOCATION_H
#define NOTE_ALLOCATION_H

#include <array>
#include "ErrorHandling.h"

namespace base::instruments::rt
{

class NoteAllocation
{
public:
   NoteAllocation() noexcept;
   static constexpr int NUM_NOTES = 128;
   static constexpr int FREE      = -1;
   int incrementVoiceIndex(std::size_t maxSize) noexcept;
   [[nodiscard]] int currentVoiceIndex() const noexcept;
   Ret<int> allocateVoice(int note, std::size_t numVoices) noexcept;

private:
   int m_currentVoiceIndex{-1};
   std::array<int, NUM_NOTES> m_noteAllocations;
};
}   // namespace base::instruments::rt


#include "NoteAllocation.inl"

#endif
