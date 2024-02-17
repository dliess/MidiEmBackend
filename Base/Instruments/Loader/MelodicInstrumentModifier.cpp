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

void MelodicInstrumentModifier::renameMelodicInstrument(std::string name) noexcept
{
   m_rMelodicInstrument.setName(std::move(name));
   m_rMelodicInstrument.unmarkAsDefaultCreated();
}

Void MelodicInstrumentModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         return findComponentIdxToPlaceNewComponent(rFactoryDataHolder, sdUuid, sdVoiceIdx).map(
            [&,this](auto componentIdx) -> void {
               MelodicInstrument::Voice voice;
               voice.components[componentIdx] =
                   MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
               m_rMelodicInstrument.m_voices.push_back(std::move(voice));
               if(!m_rMelodicInstrument.m_parameters[componentIdx].has_value())
               {
                  m_rMelodicInstrument.m_parameters[componentIdx] = MelodicInstrument::ParameterData(
                     md->description()->soundSection->engineBase(sdVoiceIdx),
                     md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
               }
               m_rMelodicInstrument.unmarkAsDefaultCreated();
            });
   });
}

Void MelodicInstrumentModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         return findComponentIdxToPlaceNewComponent(rFactoryDataHolder, sdUuid, sdVoiceIdx).and_then(
            [&,this](auto componentIdx) -> Void {
               return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).map(
                  [&,this](auto voice) -> void {
                     voice->components[componentIdx] =
                         MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
                     if(!m_rMelodicInstrument.m_parameters[componentIdx].has_value())
                     {
                        m_rMelodicInstrument.m_parameters[componentIdx] = MelodicInstrument::ParameterData(
                           md->description()->soundSection->engineBase(sdVoiceIdx),
                           md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
                     }
                     m_rMelodicInstrument.unmarkAsDefaultCreated();
                  });
            });
   });
}

Void MelodicInstrumentModifier::removeComponentFromMelodicInstrumentVoice(
   int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).and_then(
      [&](auto voice) -> Void {
         return safe_at(voice->components, componentIdx).map(
            [&](auto component) -> void {
               component->reset();
               m_rMelodicInstrument.unmarkAsDefaultCreated();
               auto it = std::ranges::find_if(m_rMelodicInstrument.m_voices, [componentIdx](auto& voice){
                  return voice.components[componentIdx].has_value();   
               });
               if(it == m_rMelodicInstrument.m_voices.end())
               {
                  m_rMelodicInstrument.m_parameters[componentIdx].reset();
               }
            });
      });
}

Void MelodicInstrumentModifier::removeVoiceFromMelodicInstrument(int voiceIdx) noexcept
{
   return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         m_rMelodicInstrument.m_voices.erase(m_rMelodicInstrument.m_voices.begin() + voiceIdx);
         m_rMelodicInstrument.unmarkAsDefaultCreated();
         for(auto it = m_rMelodicInstrument.m_parameters.begin(); it != m_rMelodicInstrument.m_parameters.end(); ++it)
         {
            if(!it->has_value())
            {
               continue;
            }
            if(!std::ranges::any_of(m_rMelodicInstrument.m_voices, [it,this](auto& voice){
               return voice.components[std::distance(m_rMelodicInstrument.m_parameters.begin(), it)].has_value();
            }))
            {
               it->reset();
            }
         }
      });
}

Void MelodicInstrumentModifier::setNoteOffsetInMelodicInstrumentComponent(
    int componentIdx, int noteOffset) noexcept
{
    return safe_at(m_rMelodicInstrument.m_parameters, componentIdx).map(
         [&](auto parameterData) -> void {
            if(parameterData->has_value())
            {
               parameterData->value().noteOffset = noteOffset;
               m_rMelodicInstrument.unmarkAsDefaultCreated();
            }
         });
}

Void MelodicInstrumentModifier::setMelodicComponentAmp(
                            int componentIdx, float amp) noexcept
{
  return safe_at(m_rMelodicInstrument.m_parameters, componentIdx).map(
      [&](auto parameterData) -> void {
         if(parameterData->has_value())
         {
            parameterData->value().amp = amp;
            m_rMelodicInstrument.unmarkAsDefaultCreated();
         }
      });
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

void MelodicInstrumentModifier::fillReferences(
    musicDevice::MusicDevice* pMusicDevice)
{
   for(int i = 0; i < m_rMelodicInstrument.m_parameters.size(); ++i)
   {
      auto& parameterData = m_rMelodicInstrument.m_parameters[i];
      if (parameterData.has_value() && parameterData->pEngineDescr == nullptr)
      {
         auto it = std::ranges::find_if(m_rMelodicInstrument.m_voices, [i, pMusicDevice](auto& voice){
            return voice.components[i].has_value() && 
            voice.components[i]->soundDeviceId == pMusicDevice->deviceId();
         });
         if(it != m_rMelodicInstrument.m_voices.end())
         {
            parameterData->pEngineDescr = pMusicDevice->description()->soundSection->engineBase(it->components[i]->sdVoiceIdx);
         }
      }
   }
}
