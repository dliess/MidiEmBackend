#include "BaseSequence.h"
#include "loguru.hpp"

using namespace base::arp;

BaseSequence::BaseSequence(std::pmr::unsynchronized_pool_resource& pool) :
    m_noteList(&pool)
{
}

void BaseSequence::push_back(int note, float velocity)
{
   //LOG_F(INFO, "ArpSequence push_back({}, {})", note, velocity);
   m_noteList.push_back({note, velocity});
}

void BaseSequence::clear() noexcept
{
   //LOG_F(INFO, "Clear ArpSequence");
   m_noteList.clear();
}

size_t BaseSequence::size() const noexcept
{
   return m_noteList.size();
}
