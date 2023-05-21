#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H

#include "EnumReflect.h"
namespace base::musicDevice::sound
{

DECLARE_ENUM(ParameterPart, uint, Commanded, LfoFrequency, LfoAmplitude,
             LfoWaveform, LfoMultiplierExp);

}   // namespace base::musicDevice::sound

#endif