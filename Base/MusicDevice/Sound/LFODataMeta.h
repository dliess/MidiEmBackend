#ifndef BASE_SOUND_LFO_DATA_META
#define BASE_SOUND_LFO_DATA_META

#include "JsonCast.h"
#include "Round1000.h"

namespace base::musicDevice::sound::lfo
{
inline void to_json(nlohmann::json& j, const Waveform& obj) { j = ~obj; }

inline void from_json(const nlohmann::json& j, Waveform& obj)
{
   obj = create_Waveform(j.get<std::string>());
}

inline void to_json(nlohmann::json& j, const LFOData& obj)
{
   if (obj.amplitude != DefaultAmplitude)
      j["amplitude"] = util::round1000(obj.amplitude);
   if (obj.frequency != DefaultFrequency)
      j["frequency"] = util::round1000(obj.frequency);
   if (obj.waveform != DefaultWaveform)
      j["waveform"] = obj.waveform;
   if (obj.multiplierExp != DefaultMultiplierExp)
      j["multiplierExp"] = obj.multiplierExp;
}

inline void from_json(const nlohmann::json& j, LFOData& obj)
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
         obj.waveform = it->get<Waveform>();
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
}   // namespace base::musicDevice::sound::lfo

#endif
