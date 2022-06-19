#ifndef UTIL_EXPAND_VARIANT_H
#define UTIL_EXPAND_VARIANT_H

#include "mpark/variant.hpp"

namespace util
{
template <typename... Ts>
[[nodiscard]] mpark::variant<Ts...> expand_variant(std::size_t i)
{
   assert(i < sizeof...(Ts));
   static constexpr mpark::variant<Ts...> table[] = {Ts{}...};
   return table[i];
}

}   // namespace util

#endif