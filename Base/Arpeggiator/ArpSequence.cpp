#include "ArpSequence.h"
#include "loguru.hpp"

using namespace base::arp;

ArpSequence::ArpSequence() :
    m_oom("ArpSequence: Out of Memory", std::pmr::null_memory_resource()),
    m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
    m_pool(&m_mbr),
    m_noteList(&m_pool)
{
}

void ArpSequence::push_back(int note, float velocity)
{
   //LOG_F(INFO, "ArpSequence push_back({}, {})", note, velocity);
   m_noteList.push_back({note, velocity});
}

void ArpSequence::clear() noexcept
{
   //LOG_F(INFO, "Clear ArpSequence");
   m_noteList.clear();
}

ArpSequence::NoteData ArpSequence::get(int idx) const noexcept
{
   auto it = m_noteList.begin();
   std::advance(it, idx);
   return *it;
}

size_t ArpSequence::size() const noexcept
{
   return m_noteList.size();
}
