#ifndef UTILS_FUNCTION_TRIV_H
#define UTILS_FUNCTION_TRIV_H

#include <cstddef>
#include <array>
#include <concepts>
#include <functional>
#include <type_traits>
#include <memory>

namespace util
{

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
             *reinterpret_cast<typename std::add_pointer<std::decay_t<F>>::type>(obj),
             args...);
      })
      {
        static_assert(sizeof(std::decay_t<F>) <= Size, "buffer not big enough for function object");
        std::construct_at(reinterpret_cast<std::decay_t<F>*>(buffer_.data()), std::forward<F>(f));
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
