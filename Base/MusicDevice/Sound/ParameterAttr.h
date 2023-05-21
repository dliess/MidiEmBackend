#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_ATTR_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_ATTR_H

#include "EnumReflect.h"
#include <nlohmann/json.hpp>
namespace base::musicDevice::sound
{

DECLARE_ENUM(ParameterAttr, uint, Commanded, LfoFrequency, LfoAmplitude,
             LfoWaveform, LfoMultiplierExp);

inline void to_json(
    nlohmann::json& j, const ParameterAttr& obj)
{
   j = ~obj;
}

inline void from_json(
    const nlohmann::json& j, ParameterAttr& obj)
{
   obj = create_ParameterAttr(j.get<std::string>());
}


}   // namespace base::musicDevice::sound

#endif