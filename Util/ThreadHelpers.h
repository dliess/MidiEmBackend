#ifndef THREADED_LOOP_H
#define THREADED_LOOP_H

#include <atomic>
#include <functional>
#include <thread>
#include <chrono>

namespace util
{
class Thread
{
public:
   Thread(){};
   template<class Function, class... Args>
   explicit Thread(Function&& f,
                         Args&&... args)
   {
      m_thread = std::thread([this, f, args...]() {
            std::invoke(f, m_terminateRequest, args...);
      });
   }

   Thread(const Thread&) = delete;
   Thread(Thread&& rhs) noexcept :
      m_thread(std::move(rhs.m_thread)), m_terminateRequest()
   {
      m_terminateRequest.exchange(rhs.m_terminateRequest);
   }
   Thread& operator=(Thread&& rhs) noexcept
   {
      m_thread = std::move(rhs.m_thread);
      m_terminateRequest.exchange(rhs.m_terminateRequest);
      return *this;
   }
   ~Thread()
   {
      m_terminateRequest.store(true);
      m_thread.join();
   }
   void join() { m_thread.join(); }

   std::thread::native_handle_type native_handle() {return m_thread.native_handle();}

protected:
   std::thread m_thread;
   std::atomic<bool> m_terminateRequest{false};
};

class ThreadedLoop : public Thread
{
public:
   template<class Function, class... Args>
   explicit ThreadedLoop(std::chrono::nanoseconds period, Function&& f,
                         Args&&... args)
   {
      m_thread = std::thread([this, period, f, args...]() {
         while (!m_terminateRequest)
         {
            auto start = std::chrono::high_resolution_clock::now();

            std::invoke(f, args...);

            auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::nanoseconds diff = end - start;
            if (diff < period)
            {
               std::this_thread::sleep_for(period - diff);
            }
         }
      });
   }

};


} // namespace util

#endif