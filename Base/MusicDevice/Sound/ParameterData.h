#ifndef BASE_SOUND_PARAMETER_DATA_H
#define BASE_SOUND_PARAMETER_DATA_H

#include "LFOData.h"
#include "StrongType.h"
#include <mpark/variant.hpp>
#include "Overload.h"
namespace base::musicDevice::sound
{

template<typename Tag>
using FloatingPointType = util::StrongType<float, Tag, util::Addable, util::Substractable, util::Multipliable, util::Dividable, util::Printable, util::Swappable>;
using Parameter = FloatingPointType<struct ParameterTag>;
using ParameterLFOAmp = FloatingPointType<struct ParameterLFOAmpTag>;
using ParameterLFOFreq = FloatingPointType<struct ParameterLFOFreqTag>;
using ParameterLFOWaveform = lfo::Waveform;
using ParameterLFOMultiplExp = util::StrongType<int, struct ParameterLFOMultiplExpTag>;
struct ParameterData
{
   float commanded{0.0};
   lfo::LFOData lfo;
};

using ParameterValue = mpark::variant<Parameter, 
                                      ParameterLFOFreq, 
                                      ParameterLFOAmp, 
                                      ParameterLFOWaveform, 
                                      ParameterLFOMultiplExp>;

inline
void setParameterData(ParameterData& pd, const ParameterValue& value) noexcept
{
   SWITCH(value)
      FCASE(Parameter, val){ pd.commanded = val.get(); },
      FCASE(ParameterLFOFreq, val){ pd.lfo.frequency = val.get(); },
      FCASE(ParameterLFOAmp, val){ pd.lfo.amplitude = val.get(); },
      FCASE(ParameterLFOWaveform, val){ pd.lfo.waveform = val; },
      FCASE(ParameterLFOMultiplExp, val){ pd.lfo.multiplierExp = val.get(); }
   END_SWITCH
}

}   // namespace base::musicDevice::sound

#include "ParameterDataMeta.h"
#endif