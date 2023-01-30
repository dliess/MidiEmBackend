#ifndef BASE_SOUND_LFO_DATA_H
#define BASE_SOUND_LFO_DATA_H

#include "EnumReflect.h"

namespace base::musicDevice::sound::lfo
{
DECLARE_ENUM(Waveform, uint, Sine = 0, Square, Triangle, Saw, Random);

constexpr float DefaultAmplitude = 0.5;
constexpr float DefaultFrequency = 0.5;
constexpr Waveform DefaultWaveform = Waveform::Sine;
constexpr int DefaultMultiplierExp = 0;
constexpr uint32_t MAX_MULTIPLIER_EXP = 7;

struct LFOData
{
   float amplitude{DefaultAmplitude};
   float frequency{DefaultFrequency};
   Waveform waveform{DefaultWaveform};
   int multiplierExp{DefaultMultiplierExp};
};

}   // namespace base::musicDevice::sound::lfo
#include "LFODataMeta.h"
#endif