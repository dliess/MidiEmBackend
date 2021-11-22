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

ArpSequence::NoteData ArpSequence::get(int idx) const noexcept
{
   auto it = m_noteList.begin();
   std::advance(it, idx % m_noteList.size());
   return *it;
}

size_t ArpSequence::size() const noexcept
{
   return m_noteList.size();
}
