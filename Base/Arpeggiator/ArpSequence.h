#ifndef BASE_ARP_SEQUENCE_H
#define BASE_ARP_SEQUENCE_H

#include <cstddef>
#include <memory_resource>
#include <list>

#include "print_alloc.h"

namespace base::arp
{
class ArpSequence
{
public:
   ArpSequence();
   void push_back(int note, float velocity);
   void clear() noexcept;
   struct NoteData
   {
      int note;
      float velocity;
   };
   NoteData get(int idx) const noexcept;
   size_t size() const noexcept;

private:
   std::byte m_stackBuf[2048];
   util::PrintAlloc m_oom;
   std::pmr::monotonic_buffer_resource m_mbr;
   std::pmr::list<NoteData> m_noteList;
};

}   // namespace base::arp

#endif