#ifndef __UTIL__FUNCTION_REF__
#define __UTIL__FUNCTION_REF__

#include <concepts>
#include <functional>

namespace util
{

template <class F> class function_ref;

template <typename R, typename... Args> class function_ref<R(Args...)>
{
public:
   constexpr function_ref() noexcept = delete;
   constexpr function_ref(const function_ref<R(Args...)> &rhs) noexcept =
       default;
   constexpr function_ref<R(Args...)> &operator      =(
       const function_ref<R(Args...)> &rhs) noexcept = default;

   template <typename F>
   requires std::invocable<F &, Args...> constexpr function_ref(F &&f) noexcept
       :
       m_typeErasedFnObj(const_cast<void *>(
           reinterpret_cast<const void *>(std::addressof(f)))),
       m_callback([](void *obj, Args... args) -> R {
          return std::invoke(
              *reinterpret_cast<typename std::add_pointer<F>::type>(obj),
              args...);
       }){};
   constexpr R operator()(Args... args) const
   {
      return m_callback(m_typeErasedFnObj, args...);
   }

   constexpr void swap(function_ref<R(Args...)> &rhs) noexcept
   {
      std::swap(m_typeErasedFnObj, rhs.m_typeErasedFnObj);
      std::swap(m_callback, rhs.m_callback);
   }

private:
   void *m_typeErasedFnObj{nullptr};
   R (*m_callback)(void *, Args...){nullptr};
};

template <typename R, typename... Args>
function_ref(R (*)(Args...)) -> function_ref<R(Args...)>;

template <typename R, typename... Args>
constexpr void swap(function_ref<R(Args...)> &lhs,
                    function_ref<R(Args...)> &rhs) noexcept
{
   lhs.swap(rhs);
}

}   // namespace util

#endif