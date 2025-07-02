#ifndef UTIL_FUNCTION_MV_H
#define UTIL_FUNCTION_MV_H

#include <cstddef>
#include <array>
#include <concepts>
#include <functional>
#include <type_traits>
#include <memory>

namespace util
{


template <size_t Size, class F> class functionMv;

template <size_t Size, typename R, typename... Args>
class functionMv<Size, R(Args...)>
{
public:
    template <typename F>
    requires (!std::is_same_v<std::decay_t<F>, functionMv> && // should not be confused with the copy constructor
          std::is_invocable_r_v<R, F&&, Args...>)
    constexpr functionMv(F &&f) noexcept(std::is_nothrow_move_constructible_v<F>)
      : 
      callback_([](void *obj, Args... args) -> R {
         return std::invoke(
             *reinterpret_cast<typename std::add_pointer<std::decay_t<F>>::type>(obj),
             args...);
      }),
      destructor_([](void* obj){
            std::destroy_at(reinterpret_cast<std::decay_t<F>*>(obj));
      }),
      move_([](void* srcObj, void* dstObj){
            std::construct_at(reinterpret_cast<std::decay_t<F>*>(dstObj), std::move(*reinterpret_cast<std::decay_t<F>*>(srcObj)));
      })      
      {
        static_assert(sizeof(std::decay_t<F>) <= Size, "buffer not big enough for functionMv object");
        std::construct_at(reinterpret_cast<std::decay_t<F>*>(buffer_.data()), std::forward<F>(f));
      }
  constexpr ~functionMv() { if(destructor_) destructor_(buffer_.data()); }
  functionMv() noexcept = default;
  functionMv& operator=(const functionMv<Size, R(Args...)>& rhs) = delete;   
  functionMv& operator=(functionMv<Size, R(Args...)>&& rhs) noexcept {
         callback_=rhs.callback_;
         destructor_=rhs.destructor_;
         move_=rhs.move_;
         if(move_) move_(rhs.buffer_.data(), buffer_.data());
         return *this;
  }
  functionMv(const functionMv<Size, R(Args...)>& rhs) = delete;
  functionMv(functionMv<Size, R(Args...)>&& rhs) noexcept : 
         callback_(rhs.callback_),
         destructor_(rhs.destructor_),
         move_(rhs.move_)
  {
         if(move_) move_(rhs.buffer_.data(), buffer_.data()); 
  }

  constexpr R operator()(Args... args)
  {
      return callback_(buffer_.data(), std::forward<Args>(args)...);
  }
  constexpr operator bool() const { return callback_ != nullptr; }
private:
   static constexpr std::size_t alignment = 16UL;
   alignas(alignment) std::array<std::byte, Size> buffer_;
   R (*callback_)(void *, Args...){nullptr};
   void (*destructor_)(void*){nullptr};
   void (*move_)(void*, void*){nullptr};
};

}   // namespace util

#endif // UTIL_FUNCTION_MV_H
