#ifndef DEVICE_PRESETS_TYPES_META_H
#define DEVICE_PRESETS_TYPES_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace base::musicDevice::sound::preset
{
#include "JsonCastNamespaceFix.h"
}

template <>
inline void to_json<base::musicDevice::sound::preset::Genre>(
    nlohmann::json& j, const base::musicDevice::sound::preset::Genre& obj)
{
   j = ~obj;
}

template <>
inline void from_json<base::musicDevice::sound::preset::Genre>(
    const nlohmann::json& j, base::musicDevice::sound::preset::Genre& obj)
{
   obj = base::musicDevice::sound::preset::create_Genre(j.get<std::string>());
}

template <>
inline void to_json<base::musicDevice::sound::preset::Category>(
    nlohmann::json& j, const base::musicDevice::sound::preset::Category& obj)
{
   j = ~obj;
}

template <>
inline void from_json<base::musicDevice::sound::preset::Category>(
    const nlohmann::json& j, base::musicDevice::sound::preset::Category& obj)
{
   obj = base::musicDevice::sound::preset::create_Category(j.get<std::string>());
}

namespace meta
{
template <>
inline auto registerMembers<base::musicDevice::sound::preset::LFOData>()
{
   return members(
       member("amplitude",
              &base::musicDevice::sound::preset::LFOData::amplitude),
       member("frequency",
              &base::musicDevice::sound::preset::LFOData::frequency),
       member("waveform", &base::musicDevice::sound::preset::LFOData::waveform),
       member("multiplierExp",
              &base::musicDevice::sound::preset::LFOData::multiplierExp));
}

template <>
inline auto registerMembers<base::musicDevice::sound::preset::ParameterData>()
{
   return members(
       member("commanded",
              &base::musicDevice::sound::preset::ParameterData::commanded),
       member("lfoData",
              &base::musicDevice::sound::preset::ParameterData::lfoData));
}

template <>
inline auto registerMembers<base::musicDevice::sound::preset::Preset>()
{
   return members(
       member("category",
              &base::musicDevice::sound::preset::Preset::category),
       member("genre",
              &base::musicDevice::sound::preset::Preset::genre),
       member("parameters",
              &base::musicDevice::sound::preset::Preset::parameters));
}

}   // namespace meta

#endif