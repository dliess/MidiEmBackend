#ifndef BASE_ARP_NOTECONTAINER_H
#define BASE_ARP_NOTECONTAINER_H

#include <list>
#include <cstddef>
#include <memory_resource>
#include "print_alloc.h"

namespace base::arp
{

class NoteContainer
{
public:
    NoteContainer() noexcept;
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
    std::pmr::list<NotePress> m_noteList;
};

} // namespace base::arp

#endif