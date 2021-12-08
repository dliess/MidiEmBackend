#ifndef DEVICE_PRESETS_TYPES_META_H
#define DEVICE_PRESETS_TYPES_META_H

#include "JsonCast.h"
#include "Meta.h"

namespace
{
constexpr double round1000(double num) { return round(num * 1000) / 1000.0; }
}   // namespace

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
   obj =
       base::musicDevice::sound::preset::create_Category(j.get<std::string>());
}

template <>
inline void to_json<base::musicDevice::sound::preset::LFOData>(
    nlohmann::json& j, const base::musicDevice::sound::preset::LFOData& obj)
{
   if (obj.amplitude !=
       base::musicDevice::sound::lfo::DefaultAmplitude)
      j["amplitude"] = round1000(obj.amplitude);
   if (obj.frequency !=
       base::musicDevice::sound::lfo::DefaultFrequency)
      j["frequency"] = round1000(obj.frequency);
   if (obj.waveform !=
       base::musicDevice::sound::lfo::DefaultWaveform)
      j["waveform"] = obj.waveform;
   if (obj.multiplierExp !=
       base::musicDevice::sound::lfo::DefaultMultiplierExp)
      j["multiplierExp"] = obj.multiplierExp;
}

template <>
inline void from_json<base::musicDevice::sound::preset::LFOData>(
    const nlohmann::json& j, base::musicDevice::sound::preset::LFOData& obj)
{
   {
      const auto it = j.find("amplitude");
      if (it != j.end())
      {
         obj.amplitude = it->get<float>();
      }
   }
   {
      const auto it = j.find("frequency");
      if (it != j.end())
      {
         obj.frequency = it->get<float>();
      }
   }
   {
      const auto it = j.find("waveform");
      if (it != j.end())
      {
         obj.waveform = it->get<base::musicDevice::sound::lfo::Waveform>();
      }
   }
   {
      const auto it = j.find("multiplierExp");
      if (it != j.end())
      {
         obj.multiplierExp = it->get<int>();
      }
   }
}

template <>
inline void to_json<base::musicDevice::sound::preset::ParameterData>(
    nlohmann::json& j,
    const base::musicDevice::sound::preset::ParameterData& obj)
{
   j["commanded"] = round1000(obj.commanded);
   if (obj.lfoData.amplitude !=
           base::musicDevice::sound::lfo::DefaultAmplitude ||
       obj.lfoData.frequency !=
           base::musicDevice::sound::lfo::DefaultFrequency ||
       obj.lfoData.waveform !=
           base::musicDevice::sound::lfo::DefaultWaveform ||
       obj.lfoData.multiplierExp !=
           base::musicDevice::sound::lfo::DefaultMultiplierExp)
   {
      j["lfo"] = obj.lfoData;
   }
}

template <>
inline void from_json<base::musicDevice::sound::preset::ParameterData>(
    const nlohmann::json& j,
    base::musicDevice::sound::preset::ParameterData& obj)
{
   {
      const auto it = j.find("commanded");
      if (it != j.end())
      {
         obj.commanded = it->get<float>();
      }
   }
   {
      const auto it = j.find("lfo");
      if (it != j.end())
      {
         obj.lfoData = it->get<base::musicDevice::sound::preset::LFOData>();
      }
   }
}

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