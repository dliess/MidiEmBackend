#include "KitInstrumentComponent.h"
using namespace base;
using namespace base::instruments::loader;

Ret<float> KitComponent::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   return safe_at(m_parameterData, parameterIdx).map(
      [parameterAttr](auto p) -> float{ 
         return getParameterData(*p, parameterAttr);
      });
   return tl::unexpected(Error::invalidParameterAttr);
}

Void KitComponent::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value) 
{
   return safe_at(m_parameterData, parameterIdx).map(
      [parameterAttr, value](auto p) -> void {
         setParameterData(*p, parameterAttr, value);
      });
}


Ret<const musicDevice::description::sound::Parameter*>
KitComponent::parameterDescription(int parameterIdx) const
{
   if (m_pSoundDevice)
   {
      return &m_pSoundDevice->parameterDescription(m_sdVoiceIdx, parameterIdx);
   }
   return tl::unexpected(Error::soundHandlerNotAvailable);
}

