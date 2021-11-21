#include "ArpSequence.h"

using namespace base::arp;

ArpSequence::ArpSequence() :
    m_oom("Out of Memory", std::pmr::null_memory_resource()),
    m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
    m_noteList(&m_mbr)
{
}

void ArpSequence::push_back(int note, float velocity)
{
   m_noteList.push_back({note, velocity});
}

void ArpSequence::clear() noexcept
{
   m_noteList.clear();
}
