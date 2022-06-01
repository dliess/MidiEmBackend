#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_PART_H

namespace base::musicDevice::sound
{
    
enum class ParameterPart
{
   Commanded = 0,
   LfoAmplitude,
   LfoFrequency,
   LfoWaveform,
   LfoMultiplierExp
};

}   // namespace base::musicDevice::sound

#endif