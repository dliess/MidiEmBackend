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

#define R_SWITCH(variant) util::match(variant,
#define R_END_SWITCH );
#define CASE(Variant_Type, name) [&,this](const Variant_Type& name)
#define FCASE(Variant_Type, name) [&](const Variant_Type& name)
#define CASE_MONOSTATE [&,this](const mpark::monostate&)

#define SWITCH(variant) {auto& vr=variant; util::match(variant,
#define END_SWITCH );}
#define CASE_1(name) [&,this](const mpark::variant_alternative_t<0, std::decay_t<decltype(vr)>>& name)
#define CASE_2(name) [&,this](const mpark::variant_alternative_t<1, std::decay_t<decltype(vr)>>& name)
#define CASE_3(name) [&,this](const mpark::variant_alternative_t<2, std::decay_t<decltype(vr)>>& name)
#define CASE_4(name) [&,this](const mpark::variant_alternative_t<3, std::decay_t<decltype(vr)>>& name)
#define CASE_5(name) [&,this](const mpark::variant_alternative_t<4, std::decay_t<decltype(vr)>>& name)
#define CASE_6(name) [&,this](const mpark::variant_alternative_t<5, std::decay_t<decltype(vr)>>& name)
#define CASE_7(name) [&,this](const mpark::variant_alternative_t<6, std::decay_t<decltype(vr)>>& name)
#define CCASE_1(name) [this](const mpark::variant_alternative_t<0, std::decay_t<decltype(vr)>>& name)
#define CCASE_2(name) [this](const mpark::variant_alternative_t<1, std::decay_t<decltype(vr)>>& name)
#define CCASE_3(name) [this](const mpark::variant_alternative_t<2, std::decay_t<decltype(vr)>>& name)
#define CCASE_4(name) [this](const mpark::variant_alternative_t<3, std::decay_t<decltype(vr)>>& name)
#define CCASE_5(name) [this](const mpark::variant_alternative_t<4, std::decay_t<decltype(vr)>>& name)
#define CCASE_6(name) [this](const mpark::variant_alternative_t<5, std::decay_t<decltype(vr)>>& name)
#define CCASE_7(name) [this](const mpark::variant_alternative_t<6, std::decay_t<decltype(vr)>>& name)

#define CASE_DEFAULT [](auto&&)

#endif