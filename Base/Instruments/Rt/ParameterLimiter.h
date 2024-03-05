#ifndef INSTRUMENTS_RT_PARAMETER_LIMITER_H
#define INSTRUMENTS_RT_PARAMETER_LIMITER_H

#include "SoundSection.h"
#include "ErrorHandling.h"
#include "ParameterData.h"

namespace base::instruments::rt
{

class ParameterLimiter
{
public:
   using ParametersDescr = std::vector<musicDevice::description::sound::Parameter>;
   explicit ParameterLimiter(const ParametersDescr& parametersDescr) noexcept : parametersDescr_(parametersDescr) {}
   Ret<float> limitValue(int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr, float value, 
                         musicDevice::sound::IncrementMode mode = musicDevice::sound::IncrementMode::Limit) const
   {
       return safe_at(parametersDescr_, parameterIdx).map([&](const auto paramDescr) {
           const auto valueRange = musicDevice::sound::getParamRangeEnd(parameterAttr, *paramDescr);
           return musicDevice::sound::limitParameterValue(value, mode, valueRange);
       });
   }
private:
   const ParametersDescr& parametersDescr_; 
};

}   // namespace base::instruments::rt
#endif
