#ifndef DEVICE_PRESETS_TYPES_META_H
#define DEVICE_PRESETS_TYPES_META_H

#include "JsonCast.h"
#include "Meta.h"

namespace base::musicDevice::sound::preset
{
inline void to_json(nlohmann::json& j, const Genre& obj) { j = ~obj; }

inline void from_json(const nlohmann::json& j, Genre& obj)
{
   obj = create_Genre(j.get<std::string>());
}

inline void to_json(nlohmann::json& j, const Category& obj) { j = ~obj; }

inline void from_json(const nlohmann::json& j, Category& obj)
{
   obj = create_Category(j.get<std::string>());
}
}   // namespace base::musicDevice::sound::preset

namespace meta
{
template <>
inline auto registerMembers<base::musicDevice::sound::preset::Preset>()
{
   return members(
       member("category", &base::musicDevice::sound::preset::Preset::category),
       member("genre", &base::musicDevice::sound::preset::Preset::genre),
       member("parameters",
              &base::musicDevice::sound::preset::Preset::parameters));
}

}   // namespace meta

#endif