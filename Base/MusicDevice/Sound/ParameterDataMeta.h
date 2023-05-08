#ifndef BASE_PARAMETER_DATA_META
#define BASE_PARAMETER_DATA_META

#include "JsonCast.h"
#include "Round1000.h"

namespace base::musicDevice::sound
{
inline void to_json(nlohmann::json& j, const ParameterData& obj)
{
   j["commanded"] = util::round1000(obj.commanded);
   if (obj.lfoData.amplitude != lfo::DefaultAmplitude ||
       obj.lfoData.frequency != lfo::DefaultFrequency ||
       obj.lfoData.waveform != lfo::DefaultWaveform ||
       obj.lfoData.multiplierExp != lfo::DefaultMultiplierExp)
   {
      j["lfo"] = obj.lfoData;
   }
}

inline void from_json(const nlohmann::json& j, ParameterData& obj)
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
         obj.lfoData = it->get<lfo::LFOData>();
      }
   }
}
}   // namespace base::musicDevice::sound
#endif
