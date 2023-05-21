#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H

#include "EnumReflect.h"
#include <nlohmann/json.hpp>
namespace base::musicDevice::sound
{

DECLARE_ENUM(ParameterPart, uint, Commanded, LfoFrequency, LfoAmplitude,
             LfoWaveform, LfoMultiplierExp);

inline void to_json(
    nlohmann::json& j, const ParameterPart& obj)
{
   j = ~obj;
}

inline void from_json(
    const nlohmann::json& j, ParameterPart& obj)
{
   obj = create_ParameterPart(j.get<std::string>());
}


}   // namespace base::musicDevice::sound

#endif