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

Ret<int> MelodicInstrumentModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Ret<int> {
         return createNewVoiceInMelodicInstrument(md, sdVoiceIdx);
   });
}

Ret<int> MelodicInstrumentModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::MusicDevice* md, int sdVoiceIdx) noexcept
{
   return findComponentIdxToPlaceNewComponent(md, sdVoiceIdx).map(
      [&,this](auto componentIdx) -> int {
         MelodicInstrument::Voice voice;
         voice.components[componentIdx] =
             MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
         m_rMelodicInstrument.m_voices.push_back(std::move(voice));
         if(!m_rMelodicInstrument.m_parameters[componentIdx].has_value())
         {
            m_rMelodicInstrument.m_parameters[componentIdx] = MelodicInstrument::ParameterData(
               { md->deviceId().deviceName(), 
                 md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) },
               md->description()->soundSection->engineBase(sdVoiceIdx)->parameters,
               md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
         }
         m_rMelodicInstrument.unmarkAsDefaultCreated();
         return int(componentIdx);
      });
}

Ret<int> MelodicInstrumentModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Ret<int> {
         return addComponentToMelodicInstrumentVoice(md, voiceIdx, sdVoiceIdx);
   });
}

Ret<int> MelodicInstrumentModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::MusicDevice* md,
    int voiceIdx, int sdVoiceIdx) noexcept
{
   return findComponentIdxToPlaceNewComponent(md, sdVoiceIdx).and_then(
      [&,this](auto componentIdx) -> Ret<int> {
         return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).map(
            [&,this](auto voice) -> int {
               voice->components[componentIdx] =
                   MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
               if(!m_rMelodicInstrument.m_parameters[componentIdx].has_value())
               {
                  m_rMelodicInstrument.m_parameters[componentIdx] = MelodicInstrument::ParameterData(
                     { md->deviceId().deviceName(), 
                       md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) },
                     md->description()->soundSection->engineBase(sdVoiceIdx)->parameters,
                     md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
               }
               m_rMelodicInstrument.unmarkAsDefaultCreated();
               return int(componentIdx);
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
               if(std::ranges::none_of(m_rMelodicInstrument.m_voices, [componentIdx](auto& voice){
                  return voice.components[componentIdx].has_value();   
               })) {
                  m_rMelodicInstrument.m_parameters[componentIdx].reset();
               }
            });
      });
}

Void MelodicInstrumentModifier::removeVoiceFromMelodicInstrument(int voiceIdx) noexcept
{
   for(int componentIdx = 0; componentIdx < m_rMelodicInstrument.m_parameters.size(); ++componentIdx) {
      removeComponentFromMelodicInstrumentVoice(voiceIdx, componentIdx);
   }
   return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).map(
      [&](auto _) {
         m_rMelodicInstrument.m_voices.erase(m_rMelodicInstrument.m_voices.begin() + voiceIdx);
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
    base::musicDevice::MusicDevice* md, int sdVoiceIdx) const
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
   return determineComponentEngineId(md, sdVoiceIdx).and_then(searchFreePlaceOrEnginePtr);
}

Ret<MelodicInstrument::ParameterData::EngineId>
MelodicInstrumentModifier::determineComponentEngineId(
   base::musicDevice::MusicDevice* md,
   int sdVoiceIdx) const
{
   return MelodicInstrument::ParameterData::EngineId { 
      md->deviceId().deviceName(), 
      md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) 
   };
}

void MelodicInstrumentModifier::fillReferences(
    musicDevice::MusicDevice* pMusicDevice)
{
   std::ranges::for_each(m_rMelodicInstrument.m_parameters, [pMusicDevice](auto& parameterData){
      if (parameterData.has_value() && 
         parameterData->parametersDescr == nullptr &&
         parameterData->engineId.mdName == pMusicDevice->deviceId().deviceName())
      {
         auto engine = safe_at(pMusicDevice->description()->soundSection->engines, 
                              parameterData->engineId.engineIdx);
         if(!engine.has_value())
         {
            spdlog::error("Engine not found in MusicDeviceDescription");
            return;
         }
         parameterData->parametersDescr = &engine.value()->parameters;
      }
   });
}

bool MelodicInstrumentModifier::isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const
{
   auto hasComponentWith = [this](const musicDevice::MusicDeviceId& deviceId) {
      return std::ranges::any_of(m_rMelodicInstrument.m_voices, [deviceId](auto& voice) {
         return std::ranges::any_of(voice.components, [deviceId](auto& component) {
            return component.has_value() ? component->soundDeviceId == deviceId : false;
         });
      });
   };
   return m_rMelodicInstrument.refCount() == 0 && 
          m_rMelodicInstrument.isDefaultCreated() && 
          hasComponentWith(pMusicDevice->deviceId());
}

void MelodicInstrumentModifier::fillDescrReferences(base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept
{
   std::ranges::for_each(m_rMelodicInstrument.m_parameters, [&](auto& parameterData){
      if (parameterData.has_value() && 
         parameterData->parametersDescr == nullptr)
      {
         rFactoryDataHolder.getDescriptionByMdName(parameterData->engineId.mdName).map(
            [&](auto description) {
               parameterData->parametersDescr = &description->soundSection->engineBase(
                  parameterData->engineId.engineIdx)->parameters;
               if(parameterData->parametersDescr->size() != parameterData->deviceParameters.size())
               {
                  spdlog::error("Device parameters size mismatch");
                  parameterData->deviceParameters.resize(parameterData->parametersDescr->size());
               }
            });
      }
   });
}
