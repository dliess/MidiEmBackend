#ifndef BASE_SOUND_PARAMETER_DATA_H
#define BASE_SOUND_PARAMETER_DATA_H

#include <magic_enum.hpp>
#include <mpark/variant.hpp>

#include "LFOData.h"
#include "Overload.h"
#include "ParameterAttr.h"
#include "SoundSection.h"
#include "StrongType.h"
#include "clip.h"
#include "Parameter.h"

namespace base::musicDevice::sound
{
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

template<class T>
struct ParameterDataCustomType
{
   T commanded{};
   lfo::LFODataCustomType<T> lfo{};
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

template<typename ParameterDataType, typename ReturnType = float>
ReturnType getParameterData(const ParameterDataType& pd,
                              ParameterAttr parameterAttr) noexcept
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         if constexpr (std::is_same_v<ReturnType, decltype(pd.commanded)>)
         {
            return pd.commanded;
         }
         else
         {
            return ReturnType(pd.commanded);
         }
      }
      case (ParameterAttr::LfoFrequency):
      {
         if constexpr (std::is_same_v<ReturnType, decltype(pd.lfo.frequency)>)
         {
            return pd.lfo.frequency;
         }
         else
         {
            return ReturnType(pd.lfo.frequency);
         }
      }
      case (ParameterAttr::LfoAmplitude):
      {
         if constexpr (std::is_same_v<ReturnType, decltype(pd.lfo.amplitude)>)
         {
            return pd.lfo.amplitude;
         }
         else
         {
            return ReturnType(pd.lfo.amplitude);
         }
      }
      case (ParameterAttr::LfoWaveform):
      {
         if constexpr (std::is_same_v<ReturnType, decltype(pd.lfo.waveform)>)
         {
            return pd.lfo.waveform;
         }
         else
         {
            return ReturnType(pd.lfo.waveform);
         }
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         if constexpr (std::is_same_v<ReturnType, decltype(pd.lfo.multiplierExp)>)
         {
            return pd.lfo.multiplierExp;
         }
         else
         {
            return ReturnType(pd.lfo.multiplierExp);
         }
      }
   }
   return ReturnType{};
}

template<typename ReturnType>
ReturnType& getParameterDataRef(auto& pd,
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
         return pd.lfo.waveform;
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         return pd.lfo.multiplierExp;
      }
      default: 
      {
         return pd.commanded;
      }
   }
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
ValueRangeEnd getParamRangeEnd(
    int voiceIdx, int parameterIdx, ParameterAttr parameterAttr,
    const ParamDescrProvider& paramDescrProvider)
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         const auto& paramDescr =
            paramDescrProvider.parameterDescription(voiceIdx, parameterIdx);
         const auto vr = paramDescr.getValueRange();
         if(vr)
         {
            return vr.value();
         }
         break;
      }
      case (ParameterAttr::LfoFrequency):
      {
         return FloatingPointRangeEnd{1.0f};
      }
      case (ParameterAttr::LfoAmplitude):
      {
         return FloatingPointRangeEnd{1.0f};
      }
      case (ParameterAttr::LfoWaveform):
      {
         return ListRangeEnd{magic_enum::enum_count<lfo::Waveform>()};
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         return ListRangeEnd{lfo::MAX_MULTIPLIER_EXP + 1};
      }
   }
   assert(false);
}

inline
float limitParameterValue(float targetVal, bool roundRobin, const ValueRangeEnd& valueRange)
{
   return R_SWITCH(valueRange)
      FCASE(ListRangeEnd, range) -> float
      {
         const auto targetlistIdx = int(targetVal);
         if (roundRobin)
         {
            if (targetlistIdx < 0) 
            {
               return float(range.get() + (targetlistIdx % range.get()));
            }
            else
            {
               return float(targetlistIdx % range.get());
            }
         }
         else
         {
            return float(util::clip(targetlistIdx, 0, int(range.get())));
         }
      },
      FCASE(FloatingPointRangeEnd, range) -> float
      {
         static constexpr float FUZZ {0.0001f};
         return util::clip(targetVal, 0.0f, range.get() - FUZZ);
      }
   R_END_SWITCH
}

}   // namespace base::musicDevice::sound

#include "ParameterDataMeta.h"
#endif