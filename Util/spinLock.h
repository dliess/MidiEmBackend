#ifndef UTIL_SPIN_LOCK_H
#define UTIL_SPIN_LOCK_H

#include <atomic>

namespace util
{

class spinLock
{
public:
   void lock() noexcept
   {
      while (m_flag.test_and_set(std::memory_order_acquire))
         ;
   }

   bool tryLock() noexcept
   {
      return !m_flag.test_and_set(std::memory_order_acquire);
   }

   void unlock()  noexcept
   {
      m_flag.clear(std::memory_order_release);
   }

private:
   std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

} // namespace util

#endif