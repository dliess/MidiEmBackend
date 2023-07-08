#ifndef UTIL_TO_INTEGRAL_H
#define UTIL_TO_INTEGRAL_H

#include <type_traits>

namespace util
{

template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type 
{
   return static_cast<typename std::underlying_type<E>::type>(e);
}

}   // namespace util

#endif