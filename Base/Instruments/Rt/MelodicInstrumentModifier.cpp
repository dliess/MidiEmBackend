#include "MelodicInstrumentModifier.h"

#include <spdlog/spdlog.h>
#include "Identifiable.h"
#include "MelodicInstrument.h"
#include "MusicDeviceDescription.h"

using namespace base::instruments::rt;


MelodicInstrumentModifier::MelodicInstrumentModifier(MelodicInstrument& rMelodicInstrument) noexcept :
    m_rMelodicInstrument(rMelodicInstrument)
{
}

Void MelodicInstrumentModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::MusicDeviceContainer& rMDContainer, int componentIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rMDContainer.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         return createNewVoiceInMelodicInstrument(componentIdx, md, sdVoiceIdx);
   });
}

Void MelodicInstrumentModifier::createNewVoiceInMelodicInstrument(
    int componentIdx,
    base::musicDevice::MusicDevice* md, int sdVoiceIdx) noexcept
{
      MelodicInstrument::Voice voice;
      voice[componentIdx].emplace(md->deviceId(), sdVoiceIdx, &md->soundHandler.value());
      m_rMelodicInstrument.m_voices.push_back(std::move(voice));
      if(!m_rMelodicInstrument.m_engines[componentIdx].has_value())
      {
         m_rMelodicInstrument.m_engines[componentIdx].emplace(
            md->description()->soundSection->engineBase(sdVoiceIdx));
      }
      return Void{};
}

Void MelodicInstrumentModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::MusicDeviceContainer& rMDContainer,
    int componentIdx, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rMDContainer.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         return addComponentToMelodicInstrumentVoice(componentIdx, md, voiceIdx, sdVoiceIdx);
   });
}

Void MelodicInstrumentModifier::addComponentToMelodicInstrumentVoice(
    int componentIdx,
    base::musicDevice::MusicDevice* md,
    int voiceIdx, int sdVoiceIdx) noexcept
{
   return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         voice->operator[](componentIdx).emplace(md->deviceId(), sdVoiceIdx, &md->soundHandler.value());
         if(!m_rMelodicInstrument.m_engines[componentIdx].has_value())
         {
            m_rMelodicInstrument.m_engines[componentIdx].emplace(
               md->description()->soundSection->engineBase(sdVoiceIdx));
         }
      });
}

Void MelodicInstrumentModifier::removeComponentFromMelodicInstrumentVoice(
   int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_rMelodicInstrument.m_voices, voiceIdx).and_then([&](auto voice) -> Void {
      return safe_at(*voice, componentIdx).map([&](auto component) -> void {
            component->reset();
            if(std::ranges::none_of(m_rMelodicInstrument.m_voices, [componentIdx](auto& voice){
               return voice[componentIdx].has_value();   
            })) {
               m_rMelodicInstrument.m_engines[componentIdx].reset();
            }
         });
      });
}

Void MelodicInstrumentModifier::removeVoiceFromMelodicInstrument(int voiceIdx) noexcept
{
   for(int componentIdx = 0; componentIdx < m_rMelodicInstrument.m_engines.size(); ++componentIdx) {
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
    return safe_at(m_rMelodicInstrument.m_engines, componentIdx).map(
         [&](auto parameterData) -> void {
            if(parameterData->has_value())
            {
               parameterData->value().noteOffset = noteOffset;
            }
         });
}

Void MelodicInstrumentModifier::setMelodicComponentAmp(
                            int componentIdx, float amp) noexcept
{
  return safe_at(m_rMelodicInstrument.m_engines, componentIdx).map(
      [&](auto parameterData) -> void {
         if(parameterData->has_value())
         {
            parameterData->value().amp = amp;
         }
      });
}

// Ret<size_t>
// MelodicInstrumentModifier::findComponentIdxToPlaceNewComponent(
//     base::musicDevice::MusicDevice* md, int sdVoiceIdx) const
// {
//    auto searchFreePlaceOrEnginePtr = [&,this](auto engineId) -> Ret<size_t>
//    {
//       auto it = std::ranges::find_if(m_rMelodicInstrument.m_parameters, [&engineId](auto& parameterData){
//          return !parameterData.has_value() ||  
//                 parameterData->engineId == engineId;
//       });
//       if (it == m_rMelodicInstrument.m_parameters.end())
//       {
//          return tl::unexpected(Error::melodicVoicesFull);
//       }
//       return std::distance(m_rMelodicInstrument.m_parameters.begin(), it);
//    };
//    return determineComponentEngineId(md, sdVoiceIdx).and_then(searchFreePlaceOrEnginePtr);
// }
//
// Ret<MelodicInstrument::ParameterData::EngineId>
// MelodicInstrumentModifier::determineComponentEngineId(
//    base::musicDevice::MusicDevice* md,
//    int sdVoiceIdx) const
// {
//    return MelodicInstrument::ParameterData::EngineId { 
//       md->deviceId().deviceName(), 
//       md->description()->soundSection->voice2EngineIdx(sdVoiceIdx) 
//    };
// }
//
// void MelodicInstrumentModifier::fillReferences(
//     musicDevice::MusicDevice* pMusicDevice)
// {
//    std::ranges::for_each(m_rMelodicInstrument.m_parameters, [pMusicDevice](auto& parameterData){
//       if (parameterData.has_value() && 
//          parameterData->pEngineDescr == nullptr &&
//          parameterData->engineId.mdName == pMusicDevice->deviceId().deviceName())
//       {
//          auto engine = safe_at(pMusicDevice->description()->soundSection->engines, 
//                               parameterData->engineId.engineIdx);
//          if(!engine.has_value())
//          {
//             spdlog::error("Engine not found in MusicDeviceDescription");
//             return;
//          }
//          parameterData->pEngineDescr = engine.value();
//       }
//    });
// }
//
// bool MelodicInstrumentModifier::isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const
// {
//    auto hasComponentWith = [this](const musicDevice::MusicDeviceId& deviceId) {
//       return std::ranges::any_of(m_rMelodicInstrument.m_voices, [deviceId](auto& voice) {
//          return std::ranges::any_of(voice.components, [deviceId](auto& component) {
//             return component.has_value() ? component->soundDeviceId == deviceId : false;
//          });
//       });
//    };
//    return m_rMelodicInstrument.refCount() == 0 && 
//           m_rMelodicInstrument.isDefaultCreated() && 
//           hasComponentWith(pMusicDevice->deviceId());
// }
