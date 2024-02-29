#ifndef UTILS_FUNCTION_H
#define UTILS_FUNCTION_H

#include <cstddef>
#include <array>
#include <concepts>
#include <functional>
#include <type_traits>
#include <memory>

namespace util
{


template <size_t Size, class F> class function;

template <size_t Size, typename R, typename... Args>
class function<Size, R(Args...)>
{
public:
    template <typename F>
    requires (!std::is_same_v<std::decay_t<F>, function> && // should not be confused with the copy constructor
          std::is_invocable_r_v<R, F&&, Args...>)
    constexpr function(F &&f) noexcept(std::is_nothrow_move_constructible_v<F>)
      : 
      callback_([](void *obj, Args... args) -> R {
         return std::invoke(
             *reinterpret_cast<typename std::add_pointer<F>::type>(obj),
             args...);
      }),
      destructor_([](void* obj){std::destroy_at(reinterpret_cast<F*>(obj));}),
      clone_([](const void* srcObj, void* dstObj){std::construct_at(reinterpret_cast<F*>(dstObj), *reinterpret_cast<const F*>(srcObj));}),
      move_([](void* srcObj, void* dstObj){std::construct_at(reinterpret_cast<F*>(dstObj), std::move(*reinterpret_cast<F*>(srcObj)));})      
      {
        static_assert(sizeof(F) <= Size, "buffer not big enough for function object");
        std::construct_at(reinterpret_cast<F*>(buffer_.data()), std::forward<F>(f));
      }
  constexpr ~function() { if(destructor_) destructor_(buffer_.data()); }
  function() noexcept = default;
  function& operator=(const function<Size, R(Args...)&>) = delete;
  function& operator=(function<Size, R(Args...)&&> rhs) noexcept { swap(rhs); return *this; }
  function(const function<Size, R(Args...)&> rhs) { clone_(rhs.buffer_.data(), buffer_.data()); }
  function(function<Size, R(Args...)&&> rhs) noexcept { move_(rhs.buffer_.data(), buffer_.data()); }

  constexpr R operator()(Args... args)
  {
      return callback_(buffer_.data(), std::forward<Args>(args)...);
  }
private:
   static constexpr std::size_t alignment = 16UL;
   alignas(alignment) std::array<std::byte, Size> buffer_;
   R (*callback_)(void *, Args...){nullptr};
   void (*destructor_)(void*){nullptr};
   void (*clone_)(const void*, void*){nullptr};
   void (*move_)(void*, void*){nullptr};
};


template <size_t Size, class F> 
class functionTriv;
template <size_t Size, typename R, typename... Args>
class functionTriv<Size, R(Args...)>
{
public:
    template <typename F>
    requires (!std::is_same_v<std::decay_t<F>, functionTriv>  // should not be confused with the copy constructor
          &&  std::is_invocable_r_v<R, F&&, Args...> 
          &&  std::is_trivially_copyable_v<std::decay_t<F>> 
          &&  std::is_trivially_destructible_v<std::decay_t<F>> 
   )
    constexpr functionTriv(F &&f) noexcept(std::is_nothrow_move_constructible_v<F>)
        : callback_([](void *obj, Args... args) -> R {
         return std::invoke(
             *reinterpret_cast<typename std::add_pointer<F>::type>(obj),
             args...);
      })
      {
        static_assert(sizeof(F) <= Size, "buffer not big enough for function object");
        std::construct_at(reinterpret_cast<F*>(buffer_.data()), std::forward<F>(f));
      }
  functionTriv() noexcept = default;

  constexpr R operator()(Args... args)
  {
      return callback_(buffer_.data(), std::forward<Args>(args)...);
  }
  constexpr operator bool() const { return callback_ != nullptr; }
private:
   static constexpr std::size_t alignment = 16UL;
   alignas(alignment) std::array<std::byte, Size> buffer_;
   R (*callback_)(void *, Args...){nullptr};
};
}   // namespace utils

#endif // UTILS_FUNCTION_H
