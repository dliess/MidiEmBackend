#ifndef BASE_ARP_SEQUENCE_H
#define BASE_ARP_SEQUENCE_H

#include <cstddef>
#include <memory_resource>
#include <vector>

#include "CallbackSignal.h"
#include "print_alloc.h"

namespace base::arp
{
class ArpSequence
{
public:
   ArpSequence();

   //CB_SIGNAL(AlgorithmChanged, Algorithm);

private:
   struct NotePress
   {
      int note;
      float velocity;
   };
   std::byte m_stackBuf[1024];
   util::PrintAlloc m_oom;
   std::pmr::monotonic_buffer_resource m_mbr;
   std::pmr::unsynchronized_pool_resource m_pool;
   std::pmr::vector<NotePress> m_noteVector;
};

}   // namespace base::arp

#endif