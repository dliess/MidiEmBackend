#ifndef ITC_QUEUE_H
#define ITC_QUEUE_H

#include <functional>
#include <queue>

#include "itcAction.h"
#include "itcActionReceiver.h"
#include "spinLock.h"
//#include <mutex>
namespace util::itc
{
class Queue
{
public:
   using value_type = std::function<void()>;
   template<typename Receiver, typename Tag, typename... Args>
   void tryPush(Receiver& receiver, ActionData<Action<Tag, Args...>> data)
   {
     // if constexpr (ActionReceiver::is<Receiver, Action<Tag, Args...>>)
      {
         push([&receiver, data]() { invokeReceiver(receiver, data); });
      }
   }
/* UNUSED at this time
   value_type pop() noexcept
   {
      m_lock.lock();
      auto res = m_queue.front();
      m_queue.pop();
      m_lock.unlock();
      return res;
   }
   void popCall() noexcept { pop()(); }
*/

   void popCallAll() noexcept
   {
      m_lock.lock();
      while (m_queue.size() > 0)
      {
         m_queue.front()();
         m_queue.pop();
      }
      m_lock.unlock();
   }

private:
   std::queue<value_type> m_queue;
   util::spinLock m_lock;
   //std::mutex m_lock;

   inline void push(const value_type& v) noexcept
   {
      m_lock.lock();
      m_queue.push(v);
      m_lock.unlock();
   }
};

} // namespace util::itc

#endif