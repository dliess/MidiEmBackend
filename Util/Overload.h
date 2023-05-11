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

template<typename Variant, typename... CBs>
auto match(Variant&& v, CBs&&... cbs)
{
    return mpark::visit(
        util::overload{ std::forward<decltype(cbs)>(cbs)... }, std::forward<decltype(v)>(v));
}

} // namespace util

#define SWITCH(variant) util::match(variant,
#define END_SWITCH );

#endif