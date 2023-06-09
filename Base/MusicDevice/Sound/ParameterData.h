#ifndef BASE_SOUND_PARAMETER_DATA_H
#define BASE_SOUND_PARAMETER_DATA_H

#include <magic_enum.hpp>
#include <mpark/variant.hpp>

#include "LFOData.h"
#include "Overload.h"
#include "ParameterAttr.h"
#include "SoundSection.h"
#include "StrongType.h"
namespace base::musicDevice::sound
{
template <typename Tag>
using FloatingPointType =
    util::StrongType<float, Tag, util::Addable, util::Substractable,
                     util::Multipliable, util::Dividable, util::Printable,
                     util::Swappable>;
using Parameter            = FloatingPointType<struct ParameterTag>;
using ParameterLFOAmp      = FloatingPointType<struct ParameterLFOAmpTag>;
using ParameterLFOFreq     = FloatingPointType<struct ParameterLFOFreqTag>;
using ParameterLFOWaveform = lfo::Waveform;
using ParameterLFOMultiplExp =
    util::StrongType<int, struct ParameterLFOMultiplExpTag>;
struct ParameterData
{
   float commanded{0.0};
   lfo::LFOData lfo;
};

using ParameterValue =
    mpark::variant<Parameter, ParameterLFOFreq, ParameterLFOAmp,
                   ParameterLFOWaveform, ParameterLFOMultiplExp>;

inline void setParameterData(ParameterData& pd,
                             const ParameterValue& value) noexcept
{
   SWITCH(value)
   FCASE(Parameter, val) { pd.commanded = val.get(); }
   , FCASE(ParameterLFOFreq, val) { pd.lfo.frequency = val.get(); }
   , FCASE(ParameterLFOAmp, val) { pd.lfo.amplitude = val.get(); }
   , FCASE(ParameterLFOWaveform, val) { pd.lfo.waveform = val; }
   , FCASE(ParameterLFOMultiplExp, val) { pd.lfo.multiplierExp = val.get(); }
   END_SWITCH
}

inline void setParameterData(ParameterData& pd, ParameterAttr parameterAttr,
                             float value) noexcept
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         pd.commanded = value;
         break;
      }
      case (ParameterAttr::LfoFrequency):
      {
         pd.lfo.frequency = value;
         break;
      }
      case (ParameterAttr::LfoAmplitude):
      {
         pd.lfo.amplitude = value;
         break;
      }
      case (ParameterAttr::LfoWaveform):
      {
         pd.lfo.waveform = static_cast<lfo::Waveform>(value);
         break;
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         pd.lfo.multiplierExp = static_cast<int>(value);
         break;
      }
   }
}

inline float getParameterData(const ParameterData& pd,
                              ParameterAttr parameterAttr) noexcept
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         return pd.commanded;
      }
      case (ParameterAttr::LfoFrequency):
      {
         return pd.lfo.frequency;
      }
      case (ParameterAttr::LfoAmplitude):
      {
         return pd.lfo.amplitude;
      }
      case (ParameterAttr::LfoWaveform):
      {
         return float(pd.lfo.waveform);
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         return float(pd.lfo.multiplierExp);
      }
   }
   return 0.0;
}

static_assert(
    std::is_same_v<Parameter, mpark::variant_alternative_t<
                                  static_cast<int>(ParameterAttr::Commanded),
                                  ParameterValue>>);
static_assert(std::is_same_v<ParameterLFOFreq,
                             mpark::variant_alternative_t<
                                 static_cast<int>(ParameterAttr::LfoFrequency),
                                 ParameterValue>>);
static_assert(std::is_same_v<ParameterLFOAmp,
                             mpark::variant_alternative_t<
                                 static_cast<int>(ParameterAttr::LfoAmplitude),
                                 ParameterValue>>);
static_assert(std::is_same_v<ParameterLFOWaveform,
                             mpark::variant_alternative_t<
                                 static_cast<int>(ParameterAttr::LfoWaveform),
                                 ParameterValue>>);
static_assert(
    std::is_same_v<ParameterLFOMultiplExp,
                   mpark::variant_alternative_t<
                       static_cast<int>(ParameterAttr::LfoMultiplierExp),
                       ParameterValue>>);

template <typename ParamDescrProvider>
std::pair<bool, float> getParamValueTypeAndRange(
    int voiceIdx, int parameterIdx, ParameterAttr parameterAttr,
    const ParamDescrProvider& paramDescrProvider)
{
   static constexpr float FUZZ {0.0001f};
   static constexpr bool IS_LIST {true};
   static constexpr bool IS_CONTINOUS_VAL {false};
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         if (const description::sound::Parameter* paramDescr =
             paramDescrProvider.parameterDescription(voiceIdx, parameterIdx); paramDescr)
         {
            return std::make_pair(
                paramDescr->type == description::sound::Parameter::Type::List,
                paramDescr->getValueRange() - FUZZ);
         }
         break;
      }
      case (ParameterAttr::LfoFrequency):
      {
         return std::make_pair(IS_CONTINOUS_VAL, 1.0 - FUZZ);
      }
      case (ParameterAttr::LfoAmplitude):
      {
         return std::make_pair(IS_CONTINOUS_VAL, 1.0 - FUZZ);
      }
      case (ParameterAttr::LfoWaveform):
      {
         return std::make_pair(IS_LIST,
                               magic_enum::enum_count<lfo::Waveform>() - 1);
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         return std::make_pair(IS_LIST, lfo::MAX_MULTIPLIER_EXP);
      }
   }
   assert(false);
}

}   // namespace base::musicDevice::sound

#include "ParameterDataMeta.h"
#endif