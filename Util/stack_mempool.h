#ifndef UTIL_STACK_MEMPOOL_H
#define UTIL_STACK_MEMPOOL_H

#include <memory_resource>
#include "print_alloc.h"

namespace util
{

template<size_t size>
class StackMempool
{
public:
    StackMempool(std::string name) :
        m_oom(std::move(name), std::pmr::null_memory_resource()),
        m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
        m_memoryPool(&m_mbr)
    {}
    std::pmr::unsynchronized_pool_resource& pool() noexcept { return m_memoryPool; }
    const std::pmr::unsynchronized_pool_resource& pool() const noexcept { return m_memoryPool; }
private:
   std::byte m_stackBuf[size];
   util::PrintAlloc m_oom;
   std::pmr::monotonic_buffer_resource m_mbr;
   std::pmr::unsynchronized_pool_resource m_memoryPool;
};

}   // namespace util

#endif