#include "NoteContainer.h"

using namespace base::arp;

NoteContainer::NoteContainer() noexcept:
    m_oom("Out of Memory", std::pmr::null_memory_resource()),
    m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
    m_pool(&m_mbr),
    m_noteList(&m_pool)
{
}