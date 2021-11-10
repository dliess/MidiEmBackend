#ifndef BASE_MODULATION_LFO_META
#define BASE_MODULATION_LFO_META

#include "JsonCast.h"

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

#endif
