#ifndef DEVICE_PRESETS_META_H
#define DEVICE_PRESETS_META_H

#include "Meta.h"

namespace base::musicDevice::sound::preset
{
#include "JsonCastNamespaceFix.h"
}

template <>
inline void to_json<base::musicDevice::sound::lfo::Waveform>(
    nlohmann::json& j, const base::musicDevice::sound::lfo::Waveform& obj)
{
   //j = ~obj;
}

template <>
inline void from_json<base::musicDevice::sound::lfo::Waveform>(
    const nlohmann::json& j, base::musicDevice::sound::lfo::Waveform& obj)
{
   //obj = create_Waveform(j.get<std::string>());
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

}   // namespace meta

#endif