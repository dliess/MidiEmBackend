#ifndef INSTRUMENTS_COMPONENT_DATA_H
#define INSTRUMENTS_COMPONENT_DATA_H

#include "InstrumentsParameterCache.h"
#include "ParameterLimiter.h"

namespace base::instruments::rt
{

struct ComponentData
{
   using ParametersDescr = std::vector<musicDevice::description::sound::Parameter>;
   using ParameterData = std::vector<musicDevice::sound::ParameterData>;
   explicit ComponentData(const ParametersDescr& parametersDescr) noexcept : 
      parameterCache(parametersDescr.size()), 
      parametersDescr(&parametersDescr) {}
   explicit ComponentData(const ParametersDescr& parametersDescr, 
                          const ParameterData& parameterData) noexcept :
      parameterCache(parameterData), 
      parametersDescr(&parametersDescr) {}
   ParameterCache parameterCache;
   const ParametersDescr* parametersDescr;
   int noteOffset{0};
   float amp{1.0f};
};

}   // namespace base::instruments::rt

#endif // ! INSTRUMENTS_COMPONENT_DATA_H
