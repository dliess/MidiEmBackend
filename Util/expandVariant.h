#ifndef UTIL_EXPAND_VARIANT_H
#define UTIL_EXPAND_VARIANT_H

#include "Variant.h"

namespace util
{
template <typename... Ts>
[[nodiscard]] dl::variant<Ts...> expand_variant(std::size_t i)
{
   assert(i < sizeof...(Ts));
   static constexpr dl::variant<Ts...> table[] = {Ts{}...};
   return table[i];
}

}   // namespace util

#endif