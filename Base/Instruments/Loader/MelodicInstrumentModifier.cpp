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
                     { md->deviceId().deviceName(), 
                       md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) },
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
                           { md->deviceId().deviceName(), 
                             md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) },
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
   auto searchFreePlaceOrEnginePtr = [&,this](auto engineId) -> Ret<size_t>
   {
      auto it = std::ranges::find_if(m_rMelodicInstrument.m_parameters, [&engineId](auto& parameterData){
         return !parameterData.has_value() ||  
                parameterData->engineId == engineId;
      });
      if (it == m_rMelodicInstrument.m_parameters.end())
      {
         return tl::unexpected(Error::melodicVoicesFull);
      }
      return std::distance(m_rMelodicInstrument.m_parameters.begin(), it);
   };
   return determineComponentEngineId(rFactoryDataHolder, sdUuid, sdVoiceIdx).
      and_then(searchFreePlaceOrEnginePtr);
}

Ret<MelodicInstrument::ParameterData::EngineId>
MelodicInstrumentModifier::determineComponentEngineId(
   base::musicDevice::factory::DataHolder& rFactoryDataHolder,
   const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).map(
      [sdVoiceIdx](auto md) -> MelodicInstrument::ParameterData::EngineId {
         return { md->deviceId().deviceName(), 
                  md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) };
      });
}

void MelodicInstrumentModifier::fillReferences(
    musicDevice::MusicDevice* pMusicDevice)
{
   std::ranges::for_each(m_rMelodicInstrument.m_parameters, [pMusicDevice](auto& parameterData){
      if (parameterData.has_value() && 
         parameterData->pEngineDescr == nullptr &&
         parameterData->engineId.mdName == pMusicDevice->deviceId().deviceName())
      {
         auto engine = safe_at(pMusicDevice->description()->soundSection->engines, 
                              parameterData->engineId.engineIdx);
         if(!engine.has_value())
         {
            spdlog::error("Engine not found in MusicDeviceDescription");
            return;
         }
         parameterData->pEngineDescr = engine.value();
      }
   });
}
