#ifndef UTIL_FUNCTION_H
#define UTIL_FUNCTION_H

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
             *reinterpret_cast<typename std::add_pointer<std::decay_t<F>>::type>(obj),
             args...);
      }),
      destructor_([](void* obj){
            std::destroy_at(reinterpret_cast<std::decay_t<F>*>(obj));
      }),
      clone_([](const void* srcObj, void* dstObj){
            if constexpr(std::is_copy_constructible_v<std::decay_t<F>>) {
               std::construct_at(reinterpret_cast<std::decay_t<F>*>(dstObj), *reinterpret_cast<const std::decay_t<F>*>(srcObj));
            } else {
               assert(false && "function object not copy constructible");
            //   std::construct_at(reinterpret_cast<std::decay_t<F>*>(dstObj), std::forward<F>(*reinterpret_cast<const std::decay_t<F>*>(srcObj)));
            }
      }),
      move_([](void* srcObj, void* dstObj){
            std::construct_at(reinterpret_cast<std::decay_t<F>*>(dstObj), std::move(*reinterpret_cast<std::decay_t<F>*>(srcObj)));
      })      
      {
        static_assert(sizeof(std::decay_t<F>) <= Size, "buffer not big enough for function object");
        std::construct_at(reinterpret_cast<std::decay_t<F>*>(buffer_.data()), std::forward<F>(f));
      }
  constexpr ~function() { if(destructor_) destructor_(buffer_.data()); }
  function() noexcept = default;
  function& operator=(const function<Size, R(Args...)>& rhs) {
         destructor_ = rhs.destructor_;
         callback_ = rhs.callback_;
         move_ = rhs.move_;
         clone_ = rhs.clone_;
         if(clone_) clone_(rhs.buffer_.data(), buffer_.data());
         return *this; 
  }
  function& operator=(function<Size, R(Args...)>&& rhs) noexcept {
         std::swap(rhs, *this); 
         return *this; 
  }
  function(const function<Size, R(Args...)>& rhs) : 
         callback_(rhs.callback_), 
         destructor_(rhs.destructor_), 
         clone_(rhs.clone_), 
         move_(rhs.move_) 
  {
         if(clone_) clone_(rhs.buffer_.data(), buffer_.data()); 
  }
  function(function<Size, R(Args...)>&& rhs) noexcept : 
         callback_(rhs.callback_),
         destructor_(rhs.destructor_),
         clone_(rhs.clone_),
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
   void (*clone_)(const void*, void*){nullptr};
   void (*move_)(void*, void*){nullptr};
};

}   // namespace util

#endif // UTIL_FUNCTION_H
