#include "InstrumentParameterHandler.h"



using namespace base::instruments::rt;

ParameterHandler::ParameterHandler(ComponentData& componentData, SdVoiceRef& sdVoiceRef) noexcept
    : m_componentData(componentData), m_sdVoiceRef(sdVoiceRef) 
{
}

Void ParameterHandler::refreshParameters() const
{
   if (!m_sdVoiceRef.soundHandler)
   {
      return tl::unexpected(Error::noSoundDeviceCached);
   }
   for (int parameterIdx = 0; parameterIdx < m_componentData.parameterCache.size(); ++parameterIdx)
   {
      for(auto parameterAttr : magic_enum::enum_values<musicDevice::sound::ParameterAttr>())
      {
         auto limitParameterValue = [&](float value) { return m_componentData.parameterLimiter.limitValue(parameterIdx, parameterAttr, value); };
         auto sendParameterValue = [&](float value) { m_sdVoiceRef.soundHandler->setParameterValue(m_sdVoiceRef.sdVoiceIdx, parameterIdx, parameterAttr, value, false); };
         if (m_componentData.parameterCache.shouldBeOverwritten(parameterIdx, parameterAttr))
         {
             m_componentData.parameterCache.getModifiedParameterValue(parameterIdx, parameterAttr).and_then(limitParameterValue).map(sendParameterValue);         
         }
      }
      m_sdVoiceRef.soundHandler->calcActualVal(m_sdVoiceRef.sdVoiceIdx, parameterIdx);
   }
   m_componentData.parameterCache.clearOverwriteList();
   return Void{};
}

Void ParameterHandler::incrementParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, musicDevice::sound::IncrementMode incrementMode)
{
   auto limitParameterValue = [&](float value) { return m_componentData.parameterLimiter.limitValue(parameterIdx, parameterAttr, value, incrementMode); };

   return m_componentData.parameterCache.getParameter(parameterIdx, parameterAttr).and_then([&,this](float actualValue) {
         return limitParameterValue(actualValue + increment).and_then([&,this](float limitedParamValue) {
                m_componentData.parameterCache.setParameter(parameterIdx, parameterAttr, limitedParamValue);
                return m_componentData.parameterCache.getModifiedParameterValue(parameterIdx, parameterAttr).
                    and_then(limitParameterValue).map([&,this](float limitedModifiedValue) {
                        m_sdVoiceRef.soundHandler->setParameterValue(m_sdVoiceRef.sdVoiceIdx, parameterIdx,
                                           parameterAttr, limitedModifiedValue);
                    });
            });
      });
}

Void ParameterHandler::incrementParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, musicDevice::sound::IncrementMode incrementMode)
{
   if (m_sdVoiceRef.soundHandler)
   {
      m_sdVoiceRef.soundHandler->incrementParameterValue(
          m_sdVoiceRef.sdVoiceIdx, parameterIdx, parameterAttr, increment, incrementMode);
      m_sdVoiceRef.soundHandler->lastplayerId = nullptr;
      m_componentData.parameterCache.dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
      return Void();
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Void ParameterHandler::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   auto limitParameterValue = [&](float value) { return m_componentData.parameterLimiter.limitValue(parameterIdx, parameterAttr, value); };

   return limitParameterValue(value).and_then([&,this](float limitedParamValue) {
          m_componentData.parameterCache.setParameter(parameterIdx, parameterAttr, limitedParamValue);
          return m_componentData.parameterCache.getModifiedParameterValue(parameterIdx, parameterAttr).
              and_then(limitParameterValue).map([&,this](float limitedModifiedValue) {
                  m_sdVoiceRef.soundHandler->setParameterValue(m_sdVoiceRef.sdVoiceIdx, parameterIdx,
                                     parameterAttr, limitedModifiedValue);
              });
      });
}

Void ParameterHandler::setParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   if (m_sdVoiceRef.soundHandler)
   {
      return m_componentData.parameterLimiter.limitValue(parameterIdx, parameterAttr, value).map([&,this](float limitedValue) {
          m_sdVoiceRef.soundHandler->setParameterValue(m_sdVoiceRef.sdVoiceIdx, parameterIdx,
                                        parameterAttr, limitedValue);
          m_sdVoiceRef.soundHandler->lastplayerId = nullptr;
          m_componentData.parameterCache.dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
      });
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}
