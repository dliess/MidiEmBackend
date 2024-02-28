#ifndef INSTRUMENTS_RT_INSTRUMENT_PARAMETER_HANDLER_H
#define INSTRUMENTS_RT_INSTRUMENT_PARAMETER_HANDLER_H


#include "ComponentData.h"
#include "SdVoiceRef.h"


namespace base::instruments::rt
{

class ParameterHandler
{
public:
   ParameterHandler(ComponentData& componentData, SdVoiceRef& sdVoiceRef) noexcept;
   Void refreshParameters() const;
   Void incrementParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, musicDevice::sound::IncrementMode incrementMode);
   Void incrementParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, musicDevice::sound::IncrementMode incrementMode);
   Void setParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float value);
   Void setParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float value);

private:
   ComponentData& m_componentData;
   SdVoiceRef& m_sdVoiceRef;
};

}   // namespace base::instruments::rt

#endif
