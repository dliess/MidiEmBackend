#ifndef BASE_SOUND_LFO_DATA_META
#define BASE_SOUND_LFO_DATA_META

#include "JsonCast.h"
#include "Round1000.h"

namespace base::musicDevice::sound
{
#include "JsonCastNamespaceFix.h"
}
namespace base::musicDevice::sound::lfo
{
#include "JsonCastNamespaceFix.h"
}

template <>
inline void to_json<base::musicDevice::sound::lfo::Waveform>(
    nlohmann::json& j, const base::musicDevice::sound::lfo::Waveform& obj)
{
   j = ~obj;
}

template <>
inline void from_json<base::musicDevice::sound::lfo::Waveform>(
    const nlohmann::json& j, base::musicDevice::sound::lfo::Waveform& obj)
{
   obj = base::musicDevice::sound::lfo::create_Waveform(j.get<std::string>());
}

template <>
inline void to_json<base::musicDevice::sound::lfo::LFOData>(
    nlohmann::json& j, const base::musicDevice::sound::lfo::LFOData& obj)
{
   if (obj.amplitude !=
       base::musicDevice::sound::lfo::DefaultAmplitude)
      j["amplitude"] = util::round1000(obj.amplitude);
   if (obj.frequency !=
       base::musicDevice::sound::lfo::DefaultFrequency)
      j["frequency"] = util::round1000(obj.frequency);
   if (obj.waveform !=
       base::musicDevice::sound::lfo::DefaultWaveform)
      j["waveform"] = obj.waveform;
   if (obj.multiplierExp !=
       base::musicDevice::sound::lfo::DefaultMultiplierExp)
      j["multiplierExp"] = obj.multiplierExp;
}

template <>
inline void from_json<base::musicDevice::sound::lfo::LFOData>(
    const nlohmann::json& j, base::musicDevice::sound::lfo::LFOData& obj)
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

#endif
