#ifndef BASE_SOUND_PARAMETER_DATA_H
#define BASE_SOUND_PARAMETER_DATA_H

#include "LFOData.h"

namespace base::musicDevice::sound
{

ParameterType = float;
struct ParameterData
{
   float commanded{0.0};
   lfo::LFOData lfoData;
};

ParameterValue = mpark::variant<float, > 

}   // namespace base::musicDevice::sound

#include "ParameterDataMeta.h"
#endif