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
   using Engine = musicDevice::description::sound::EngineBase;
   explicit ParameterLimiter(const Engine* pEngineDescr) noexcept : pEngineDescr(pEngineDescr) {}
   Ret<float> limitValue(int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr, float value, 
                         musicDevice::sound::IncrementMode mode = musicDevice::sound::IncrementMode::Limit) const
   {
       return safe_at(pEngineDescr->parameters, parameterIdx).map([&](const auto paramDescr) {
           const auto valueRange = musicDevice::sound::getParamRangeEnd(parameterAttr, *paramDescr);
           return musicDevice::sound::limitParameterValue(value, mode, valueRange);
       });
   }
private:
   const Engine* pEngineDescr; 
};

}   // namespace base::instruments::rt
#endif
