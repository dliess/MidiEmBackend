#ifndef DEVICE_PRESETS_SETTINGS_META_H
#define DEVICE_PRESETS_SETTINGS_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace base::musicDevice::sound::preset::settings
{
#include "JsonCastNamespaceFix.h"
}

namespace meta
{
template <>
inline auto registerMembers<base::musicDevice::sound::preset::settings::LFOData>()
{
   return members(
       member("amplitude",
              &base::musicDevice::sound::preset::settings::LFOData::amplitude),
       member("frequency",
              &base::musicDevice::sound::preset::settings::LFOData::frequency),
       member("waveform", &base::musicDevice::sound::preset::settings::LFOData::waveform),
       member("multiplierExp",
              &base::musicDevice::sound::preset::settings::LFOData::multiplierExp));
}

template <>
inline auto registerMembers<base::musicDevice::sound::preset::settings::ParameterData>()
{
   return members(
       member("commanded",
              &base::musicDevice::sound::preset::settings::ParameterData::commanded),
       member("lfoData",
              &base::musicDevice::sound::preset::settings::ParameterData::lfoData));
}

template <>
inline auto registerMembers<base::musicDevice::sound::preset::settings::Preset>()
{
   return members(
       member("category",
              &base::musicDevice::sound::preset::settings::Preset::category),
       member("genre",
              &base::musicDevice::sound::preset::settings::Preset::genre),
       member("parameters",
              &base::musicDevice::sound::preset::settings::Preset::parameters));
}

}   // namespace meta

#endif