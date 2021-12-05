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

template <>
inline void to_json<base::musicDevice::sound::preset::LFOData>(
    nlohmann::json& j, const base::musicDevice::sound::preset::LFOData& obj)
{
   if(obj.amplitude != base::musicDevice::sound::preset::LFOData::DefaultAmplitude)
       j["amplitude"] = obj.amplitude;
   if(obj.frequency != base::musicDevice::sound::preset::LFOData::DefaultFrequency)
       j["frequency"] = obj.frequency;
   if(obj.waveform != base::musicDevice::sound::preset::LFOData::DefaultWaveform)
       j["waveform"] = obj.waveform;
   if(obj.multiplierExp != base::musicDevice::sound::preset::LFOData::DefaultMultiplierExp)
       j["multiplierExp"] = obj.multiplierExp;   
}

template <>
inline void from_json<base::musicDevice::sound::preset::LFOData>(
    const nlohmann::json& j, base::musicDevice::sound::preset::LFOData& obj)
{
   {
       const auto it = j.find("amplitude");
       if(it != j.end())
       {
           obj.amplitude = it->get<float>();
       }
   }
   {
       const auto it = j.find("frequency");
       if(it != j.end())
       {
           obj.frequency = it->get<float>();
       }
   }
   {
       const auto it = j.find("waveform");
       if(it != j.end())
       {
           obj.waveform = it->get<base::musicDevice::sound::lfo::Waveform>();
       }
   }
   {
       const auto it = j.find("multiplierExp");
       if(it != j.end())
       {
           obj.multiplierExp = it->get<int>();
       }
   }
}

namespace meta
{

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