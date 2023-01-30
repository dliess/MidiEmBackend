#ifndef BASE_PARAMETER_DATA_META
#define BASE_PARAMETER_DATA_META

#include "JsonCast.h"
#include "Round1000.h"

template <>
inline void to_json<base::musicDevice::sound::ParameterData>(
    nlohmann::json& j,
    const base::musicDevice::sound::ParameterData& obj)
{
   j["commanded"] = util::round1000(obj.commanded);
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
inline void from_json<base::musicDevice::sound::ParameterData>(
    const nlohmann::json& j,
    base::musicDevice::sound::ParameterData& obj)
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
         obj.lfoData = it->get<base::musicDevice::sound::lfo::LFOData>();
      }
   }
}

#endif
