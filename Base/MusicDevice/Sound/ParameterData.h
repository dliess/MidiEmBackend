#ifndef BASE_SOUND_PARAMETER_DATA_H
#define BASE_SOUND_PARAMETER_DATA_H

#include "LFOData.h"
#include "StrongType.h"
#include <mpark/variant.hpp>
#include "Overload.h"
#include "ParameterAttr.h"
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

static_assert(std::is_same_v <Parameter, 
   mpark::variant_alternative_t<static_cast<int>(ParameterAttr::Commanded), ParameterValue>>);
static_assert(std::is_same_v <ParameterLFOFreq, 
   mpark::variant_alternative_t<static_cast<int>(ParameterAttr::LfoFrequency), ParameterValue>>);
static_assert(std::is_same_v <ParameterLFOAmp, 
   mpark::variant_alternative_t<static_cast<int>(ParameterAttr::LfoAmplitude), ParameterValue>>);
static_assert(std::is_same_v <ParameterLFOWaveform, 
   mpark::variant_alternative_t<static_cast<int>(ParameterAttr::LfoWaveform), ParameterValue>>);
static_assert(std::is_same_v <ParameterLFOMultiplExp, 
   mpark::variant_alternative_t<static_cast<int>(ParameterAttr::LfoMultiplierExp), ParameterValue>>);

}   // namespace base::musicDevice::sound

#include "ParameterDataMeta.h"
#endif