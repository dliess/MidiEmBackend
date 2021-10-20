#ifndef UTIL_OVERLOAD_H
#define UTIL_OVERLOAD_H

namespace util
{

#define VARIANT_GET(variant, member) \
   mpark::visit(                    \
      util::overload{               \
         [](auto&& v) -> auto { return v.member; }}, (variant))

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

} // namespace util

#endif