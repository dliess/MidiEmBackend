#ifndef UTIL_DOUBLE_BUFFER_H
#define UTIL_DOUBLE_BUFFER_H

#include <mutex>
#include "function_ref.h"

namespace util
{

template <class Data> class DoubleBuffer
{
public:
   void withRtLocked(function_ref<void(const Data&)> cb)
   {
      std::lock_guard<std::mutex> guard(m_mutex);
      cb(m_data[rtIndex()]);
   }
   void withNonRtLocked(function_ref<void(Data&)> cb)
   {
      cb(m_data[nonRtIndex()]);
      {
         std::lock_guard<std::mutex> guard(m_mutex);
         ++m_rtIndex;
      }
      cb(m_data[nonRtIndex()]);
   }
   const Data& nonRt() const { return m_data[nonRtIndex()]; }

private:
   int m_rtIndex{0};
   Data m_data[2];
   std::mutex m_mutex;
   [[nodiscard]] constexpr int rtIndex() const noexcept{ return m_rtIndex & 1; };
   [[nodiscard]] constexpr int nonRtIndex() const noexcept{ return (m_rtIndex + 1) & 1; };
};

}   // namespace util
#endif