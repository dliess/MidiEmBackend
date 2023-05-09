#ifndef IDENTIFIABLE_META_H
#define IDENTIFIABLE_META_H

#include "JsonCast.h"

// TODO: somehow no approach worked to overrule std::array serialization


// Approach 1
/*
template<>
inline void to_json<util::Identifiable::UUID>(
   nlohmann::json& j,
   const util::Identifiable::UUID& obj)
{
   j = nlohmann::json(std::string(obj.begin(), obj.end()));
}

template<>
inline void from_json<util::Identifiable::UUID>(
   const nlohmann::json& j,
   util::Identifiable::UUID& obj)
{
   auto str = meta::deserialize<std::string>(j);
   std::copy_n(str.begin(), obj.size(), obj.begin());
}
*/

// Approach 2

/*
namespace meta
{

template <>
nlohmann::json serialize_basic(const util::Identifiable::UUID& obj)
{
    return nlohmann::json(std::string(obj.begin(), obj.end()));
}

template <>
void deserialize_basic(util::Identifiable::UUID& obj, const nlohmann::json& j)
{
   auto str = meta::deserialize<std::string>(j);
   std::copy_n(str.begin(), obj.size(), obj.begin());
}
} // namespace meta
*/


#endif