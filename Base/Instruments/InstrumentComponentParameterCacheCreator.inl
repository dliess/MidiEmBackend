#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_INL

#include "InstrumentComponentParameterCacheCreator.h"

namespace base::instruments
{
template <typename ParameterChangeEmitter>
MelodicInstrumentsParameterCacheCreator<ParameterChangeEmitter>::
    MelodicInstrumentsParameterCacheCreator(
        MelodicInstrument& rMelodicInstrument,
        const base::musicDevice::factory::DataHolder& rFactoryDataHolder,
        ParameterChangeEmitter& rParameterChangeEmitter) :
    m_rMelodicInstrument(rMelodicInstrument),
    m_rFactoryDataHolder(rFactoryDataHolder),
    m_rParameterChangeEmitter(rParameterChangeEmitter)
{
}

template <typename ParameterChangeEmitter>
void MelodicInstrumentsParameterCacheCreator<
    ParameterChangeEmitter>::initParameterCaches()
{
   for (size_t componentIdx = 0; componentIdx < 4; ++componentIdx)
   {
      auto pComponent = getFirstComponent(componentIdx);
      if(!pComponent) continue;
      auto parameterache = getParameterCacheForComponent(componentIdx);
      if (!parameterache)
      {
         parameterache.reset(createParameterCache(
             m_rFactoryDataHolder
                 .getDescription(pComponent->m_soundDeviceId.deviceName())
                 .get(),
             pComponent->m_sdVoiceIndex));
         parameterache->onDataChangedUI(
             [&, this](int parameterId,
                       musicDevice::sound::ParameterAttr parameterAttr,
                       float value) {
                m_rParameterChangeEmitter->emitMelodicInstrumentParamChanged(
                    instr.id(), componentIdx, parameterId, parameterAttr,
                    value);
             });
      }

      for (auto& voice : m_rMelodicInstrument.voices())
      {
         if (voice.components.containsComponent(componentIdx) &&
             voice.components[componentIdx].m_pParameterCache == nullptr)
         {
            voice.components[componentIdx].m_pParameterCache.reset(
                parameterache);
         }
      }
   }
}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<
    ParameterChangeEmitter>::createParameterCacheForNextComponentInNextVoice()
{
   return nullptr;
}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<ParameterChangeEmitter>::
    createParameterCacheForNextComponentInVoice(size_t voiceIdx)
{
   return nullptr;
   /*
       m_rMelodicInstrument.voices().at(voiceIdx)
       return createParameterCache(
                       m_rFactoryDataHolder
                           .getDescription(component.m_soundDeviceId.deviceName())
                           .get(),
                       component.m_sdVoiceIndex)
   */
}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<
    ParameterChangeEmitter>::getParameterCacheForComponent(size_t componentIdx)
{
   for (const auto& voice : m_rMelodicInstrument.voices())
   {
      if (voice.components.containsComponent(componentIdx))
      {
         // return voice.components[componentIdx].m_pParameterCache;
      }
   }
   return nullptr;
}

}   // namespace base::instruments

#endif
