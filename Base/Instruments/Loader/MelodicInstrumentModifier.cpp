#include "MelodicInstrumentModifier.h"

#include <spdlog/spdlog.h>
#include "Identifiable.h"
#include "MelodicInstrument.h"
#include "MusicDeviceDescription.h"

using namespace base::instruments::loader;


MelodicInstrumentModifier::MelodicInstrumentModifier(MelodicInstrument& rMelodicInstrument) noexcept :
    m_rMelodicInstrument(rMelodicInstrument)
{
}

Void MelodicInstrumentModifier::renameMelodicInstrument(std::string name) noexcept
{
   m_rMelodicInstrument.setName(std::move(name));
   m_rMelodicInstrument.unmarkAsDefaultCreated();
   return Void();
}

Ret<size_t>
MelodicInstrumentModifier::findComponentIdxToPlaceNewComponent(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   auto searchFreePlaceOrEnginePtr = [this, &rFactoryDataHolder, sdUuid, sdVoiceIdx](auto enginePtr) -> Ret<size_t>
   {
      auto it = std::ranges::find_if(m_rMelodicInstrument.m_parameters, [enginePtr](auto& parameterData){
         return !parameterData.has_value() ||  
                parameterData->pEngineDescr == enginePtr;
      });
      if (it == m_rMelodicInstrument.m_parameters.end())
      {
         return tl::unexpected(Error::melodicVoicesFull);
      }
      return std::distance(m_rMelodicInstrument.m_parameters.begin(), it);
   };
   return determineComponentEngineType(rFactoryDataHolder, sdUuid, sdVoiceIdx).
      and_then(searchFreePlaceOrEnginePtr);
}

Ret<const MelodicInstrumentModifier::Engine*>
MelodicInstrumentModifier::determineComponentEngineType(
   base::musicDevice::factory::DataHolder& rFactoryDataHolder,
   const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   return rFactoryDataHolder.getDescription(sdUuid).map([sdVoiceIdx](auto descr) {
      return descr->soundSection->engineBase(sdVoiceIdx);
   });
}

/*
const Component* MelodicInstrument::getFirstComponent(
   const MelodicInstrument& melodicInstrument, size_t componentIdx) const
{
   for (const auto& voice : melodicInstrument.m_voices)
   {
      if (voice.components[componentIdx].has_value())
      {
         return &voice.components[componentIdx].value();
      }
   }
   return nullptr;
}

Ret<std::pair<musicDevice::MusicDeviceName, int>>
MelodicInstrumentsParameterCacheCreator::getComponentEngineType(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const MelodicInstrument& melodicInstrument, size_t componentIdx) const
{
   auto pComponent = melodicInstrument.getFirstComponent(componentIdx);
   if (!pComponent)
   {
      return std::nullopt;
   }
   const auto mdName = pComponent->m_soundDeviceId.deviceName();
   const auto descr  = rFactoryDataHolder.getDescription(
       pComponent->m_soundDeviceId.deviceName());
   if (!descr)
   {
      return std::nullopt;
   }
   return std::make_pair(
       mdName, descr->soundSection->voice2EngineIdx(pComponent->m_sdVoiceIdx));
}
*/
