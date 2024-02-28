#ifndef INSTRUMENTS_COMPONENT_DATA_H
#define INSTRUMENTS_COMPONENT_DATA_H

#include "InstrumentsParameterCache.h"
#include "ParameterLimiter.h"

namespace base::instruments::rt
{

struct ComponentData
{
   ComponentData(const musicDevice::description::sound::EngineBase* pEngineDescr) noexcept : 
      parameterCache(pEngineDescr->parameters.size()), 
      parameterLimiter(pEngineDescr) {}
   ParameterCache parameterCache;
   ParameterLimiter parameterLimiter;
   int noteOffset{0};
   float amp{1.0f};
};

}   // namespace base::instruments::rt

#endif // ! INSTRUMENTS_COMPONENT_DATA_H
