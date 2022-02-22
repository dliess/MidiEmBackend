#include "ArpSequence.h"
#include "spdlog/spdlog.h"

using namespace base::arp;

ArpSequence::ArpSequence(std::pmr::unsynchronized_pool_resource& pool) :
    m_noteList(&pool)
{
}

void ArpSequence::push_back(int note, float velocity)
{
   //spdlog::info( "ArpSequence push_back({}, {})", note, velocity);
   m_noteList.push_back({note, velocity});
}

void ArpSequence::clear() noexcept
{
   //spdlog::info( "----------------Clear ArpSequence----------------");
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
