#ifndef UTIL_ARRAY_COUNT_H
#define UTIL_ARRAY_COUNT_H

#include <array>
#include <type_traits>

namespace util
{
template <typename T> struct is_array : std::false_type
{
};

template <typename T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type
{
};

template <typename T, typename Enable = void> struct array_count
{
   static constexpr int value = 1;
};

template <typename T>
struct array_count<T, typename std::enable_if<std::is_array<T>::value>::type>
{
   static constexpr int value =
       sizeof(T) /
       sizeof(typename std::remove_pointer_t<typename std::decay_t<T>>);
};

template <typename T>
inline constexpr int array_count_v = array_count<T>::value;

}   // namespace util
#endif
