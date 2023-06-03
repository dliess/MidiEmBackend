#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_INL

#include "InstrumentComponentParameterCacheCreator.h"

namespace base::instruments
{
inline MelodicInstrumentsParameterCacheCreator::
    MelodicInstrumentsParameterCacheCreator(
        musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}

template <typename ParameterChangeEmitter>
void MelodicInstrumentsParameterCacheCreator::initParameterCaches(
    MelodicInstrument& melodicInstrument,
    ParameterChangeEmitter& rParameterChangeEmitter)
{
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto pComponent = melodicInstrument.getFirstComponent(componentIdx);
      if (!pComponent)
         continue;
      auto parameterCache =
          getParameterCacheForComponent(melodicInstrument, componentIdx);
      if (!parameterCache)
      {
         parameterCache = createParameterCache(
             m_rFactoryDataHolder
                 .getDescription(pComponent->m_soundDeviceId.deviceName())
                 .get(),
             pComponent->m_sdVoiceIndex);
      }
      const auto uuid = melodicInstrument.id();
      parameterCache->onDataChangedUI(
          [uuid, componentIdx, &rParameterChangeEmitter](
              int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
              float value) {
             rParameterChangeEmitter.emitMelodicInstrumentParamChanged(
                 uuid, componentIdx, parameterId, parameterAttr, value);
          });

      for (auto& voice : melodicInstrument.voices())
      {
         if (voice.components.containsComponent(componentIdx) &&
             voice.components[componentIdx]->m_pParameterCache == nullptr)
         {
            voice.components[componentIdx]->m_pParameterCache = parameterCache;
         }
      }
   }
}

inline std::shared_ptr<ParameterCache>
MelodicInstrumentsParameterCacheCreator::getParameterCacheForComponent(
    const MelodicInstrument& melodicInstrument, size_t componentIdx) const
{
   for (const auto& voice : melodicInstrument.voices())
   {
      if (voice.components.containsComponent(componentIdx) &&
          voice.components[componentIdx]->m_pParameterCache != nullptr)
      {
         return voice.components[componentIdx]->m_pParameterCache;
      }
   }
   return nullptr;
}

template <typename ParameterChangeEmitter>
std::shared_ptr<ParameterCache> MelodicInstrumentsParameterCacheCreator::
    createParameterCacheForNextMatchingComponentInNextVoice(
        const MelodicInstrument& melodicInstrument,
        const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
        ParameterChangeEmitter& rParameterChangeEmitter) const
{
   auto newComponentIdx = findComponentIdxToPlaceNewComponent(
       melodicInstrument, sdUuid, sdVoiceIdx);
   if (!newComponentIdx)
   {
      return nullptr;
   }
   return createParameterCacheForComponent(melodicInstrument,
                                           newComponentIdx.value(), sdUuid,
                                           sdVoiceIdx, rParameterChangeEmitter);
}

template <typename ParameterChangeEmitter>
std::shared_ptr<ParameterCache> MelodicInstrumentsParameterCacheCreator::
    createParameterCacheForNextMatchingComponentInVoice(
        const MelodicInstrument& melodicInstrument, int voiceIdx,
        const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
        ParameterChangeEmitter& rParameterChangeEmitter) const
{
   auto newComponentIdx = findComponentIdxToPlaceNewComponentInVoice(
       melodicInstrument, voiceIdx, sdUuid, sdVoiceIdx);
   if (!newComponentIdx)
   {
      return nullptr;
   }
   return createParameterCacheForComponent(melodicInstrument,
                                           newComponentIdx.value(), sdUuid,
                                           sdVoiceIdx, rParameterChangeEmitter);
}

template <typename ParameterChangeEmitter>
[[nodiscard]] std::shared_ptr<ParameterCache>
MelodicInstrumentsParameterCacheCreator::createParameterCacheForComponent(
    const MelodicInstrument& melodicInstrument, int componentIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    ParameterChangeEmitter& rParameterChangeEmitter) const
{
   auto parameterCache =
       getParameterCacheForComponent(melodicInstrument, componentIdx);
   if (!parameterCache)
   {
      parameterCache = createParameterCache(
          m_rFactoryDataHolder.getDescription(sdUuid), sdVoiceIdx);
   }
   const auto uuid = melodicInstrument.id();
   parameterCache->onDataChangedUI(
       [uuid, componentIdx, &rParameterChangeEmitter](
           int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
           float value) {
          rParameterChangeEmitter.emitMelodicInstrumentParamChanged(
              uuid, componentIdx, parameterId, parameterAttr, value);
       });
   return parameterCache;
}

inline std::optional<size_t>
MelodicInstrumentsParameterCacheCreator::findComponentIdxToPlaceNewComponent(
    const MelodicInstrument& melodicInstrument,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   auto engineType = determineComponentEngineType(sdUuid, sdVoiceIdx);
   if (!engineType)
   {
      return std::nullopt;
   }
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto componentEngineType =
          getComponentEngineType(melodicInstrument, componentIdx);
      if (!componentEngineType)
      {
         return componentIdx;
      }
      if (componentEngineType.value() == engineType.value())
      {
         return componentIdx;
      }
   }
   return std::nullopt;
}

inline std::optional<size_t> MelodicInstrumentsParameterCacheCreator::
    findComponentIdxToPlaceNewComponentInVoice(
        const MelodicInstrument& melodicInstrument, int voiceIdx,
        const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   auto engineType = determineComponentEngineType(sdUuid, sdVoiceIdx);
   if (!engineType)
   {
      return std::nullopt;
   }
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto componentEngineType =
          getComponentEngineType(melodicInstrument, componentIdx);
      if (melodicInstrument.voices().at(voiceIdx).components.containsComponent(
              componentIdx))
      {
         continue;
      }
      if (!componentEngineType)
      {
         return componentIdx;
      }
      if (componentEngineType.value() == engineType.value())
      {
         return componentIdx;
      }
   }
   return std::nullopt;
}

inline std::optional<std::pair<musicDevice::MusicDeviceName, int>>
MelodicInstrumentsParameterCacheCreator::getComponentEngineType(
    const MelodicInstrument& melodicInstrument, size_t componentIdx) const
{
   auto pComponent = melodicInstrument.getFirstComponent(componentIdx);
   if (!pComponent)
   {
      return std::nullopt;
   }
   const auto mdName = pComponent->m_soundDeviceId.deviceName();
   const auto descr  = m_rFactoryDataHolder.getDescription(
       pComponent->m_soundDeviceId.deviceName());
   if (!descr)
   {
      return std::nullopt;
   }
   return std::make_pair(mdName, descr->soundSection->voice2EngineIdx(
                                     pComponent->m_sdVoiceIndex));
}

inline std::optional<std::pair<musicDevice::MusicDeviceName, int>>
MelodicInstrumentsParameterCacheCreator::determineComponentEngineType(
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   auto musicDeviceId = m_rFactoryDataHolder.getMdIdByUUID(sdUuid);
   if (!musicDeviceId)
   {
      return std::nullopt;
   }
   const auto descr = m_rFactoryDataHolder.getDescription(sdUuid);
   if (!descr)
   {
      return std::nullopt;
   }
   return std::make_pair(musicDeviceId->deviceName(),
                         descr->soundSection->voice2EngineIdx(sdVoiceIdx));
}

//------------------------------------------------------------------------------------

inline KitInstrumentsParameterCacheCreator::KitInstrumentsParameterCacheCreator(
    musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}

template <typename ParameterChangeEmitter>
void KitInstrumentsParameterCacheCreator::initParameterCaches(
    KitInstrument& kitInstrument,
    ParameterChangeEmitter& rParameterChangeEmitter)
{
   kitInstrument.forEachComponentExt([&, this](auto& component, int voiceIdx,
                                               int componentIdx) {
      component.m_pParameterCache = createParameterCache(
          m_rFactoryDataHolder
              .getDescription(component.m_soundDeviceId.deviceName())
              .get(),
          component.m_sdVoiceIndex);
      const auto uuid = kitInstrument.id();
      component.parameterCache()->onDataChangedUI(
          [uuid, voiceIdx, componentIdx, &rParameterChangeEmitter](
              int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
              float value) {
             rParameterChangeEmitter.emitKitInstrumentParamChanged(
                 uuid, voiceIdx, componentIdx, parameterId, parameterAttr,
                 value);
          });
   });
}

template <typename ParameterChangeEmitter>
[[nodiscard]] std::shared_ptr<ParameterCache>
KitInstrumentsParameterCacheCreator::
    createParameterCacheForNewComponentInNextVoice(
        const KitInstrument& kitInstrument,
        const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
        ParameterChangeEmitter& rParameterChangeEmitter) const
{
   const size_t voiceIdx     = kitInstrument.voices().size();
   const size_t componentIdx = 0;
   return createParameterCacheForComponent(kitInstrument, voiceIdx,
                                           componentIdx, sdUuid, sdVoiceIdx,
                                           rParameterChangeEmitter);
}

template <typename ParameterChangeEmitter>
[[nodiscard]] std::shared_ptr<ParameterCache>
KitInstrumentsParameterCacheCreator::createParameterCacheForNewComponentInVoice(
    const KitInstrument& kitInstrument, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    ParameterChangeEmitter& rParameterChangeEmitter) const
{
   const size_t componentIdx =
       kitInstrument.voices().at(voiceIdx).components.size();
   return createParameterCacheForComponent(kitInstrument, voiceIdx,
                                           componentIdx, sdUuid, sdVoiceIdx,
                                           rParameterChangeEmitter);
}

template <typename ParameterChangeEmitter>
[[nodiscard]] std::shared_ptr<ParameterCache>
KitInstrumentsParameterCacheCreator::createParameterCacheForComponent(
    const KitInstrument& kitInstrument, int voiceIdx, int componentIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    ParameterChangeEmitter& rParameterChangeEmitter) const
{
   auto parameterCache = createParameterCache(
       m_rFactoryDataHolder.getDescription(sdUuid), sdVoiceIdx);
   if (!parameterCache)
   {
      return nullptr;
   }
   const auto uuid = kitInstrument.id();
   parameterCache->onDataChangedUI(
       [uuid, voiceIdx, componentIdx, &rParameterChangeEmitter](
           int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
           float value) {
          rParameterChangeEmitter.emitKitInstrumentParamChanged(
              uuid, voiceIdx, componentIdx, parameterId, parameterAttr, value);
       });
   return parameterCache;
}

}   // namespace base::instruments

#endif
