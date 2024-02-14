#include "MelodicInstrumentsModifier.h"
#include "MelodicInstrumentModifier.h"

#include <spdlog/spdlog.h>
#include "Identifiable.h"


using namespace base::instruments::loader;

Ret<MelodicInstruments::iterator> 
MelodicInstrumentsModifier::getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept
{
   auto instrumentIt = std::ranges::find_if(                                 
       m_rMelodicInstruments,                                                
       [&instrumentUuid](const MelodicInstrument& instr) {                   
          return instr.id() == instrumentUuid;                               
       });                                                                   
   if (instrumentIt == m_rMelodicInstruments.end())                          
   {                                                                         
      return tl::unexpected(Error::uuidNotFound);                                                            \
   }
   return instrumentIt;
}

MelodicInstrumentsModifier::MelodicInstrumentsModifier(MelodicInstruments& rMelodicInstruments) noexcept :
    m_rMelodicInstruments(rMelodicInstruments)
{
}

void MelodicInstrumentsModifier::insertMelodicInstrument(
    MelodicInstrument melodicInstrument) noexcept
{
   m_rMelodicInstruments.push_back(std::move(melodicInstrument));
}

Void MelodicInstrumentsModifier::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   return getInstrument(instrumentId).map([this](auto instrumentIt) -> void {
      m_rMelodicInstruments.erase(instrumentIt);
   });
}

Void MelodicInstrumentsModifier::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   return getInstrument(instrumentId).map([this, name = std::move(name)](auto instrumentIt) -> void {
      instrumentIt->setName(std::move(name));
      instrumentIt->unmarkAsDefaultCreated();
   });
}


Void MelodicInstrumentsModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
         return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
            [&,this](auto md) -> Void {
               return MelodicInstrumentModifier(*instrumentIt)
                                      .findComponentIdxToPlaceNewComponent(rFactoryDataHolder, sdUuid, sdVoiceIdx).map(
                  [&,this](auto componentIdx) -> void {
                     MelodicInstrument::Voice voice;
                     voice.components[componentIdx] =
                         MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
                     instrumentIt->m_voices.push_back(std::move(voice));
                     if(!instrumentIt->m_parameters[componentIdx].has_value())
                     {
                        instrumentIt->m_parameters[componentIdx] = MelodicInstrument::ParameterData(
                           md->description()->soundSection->engineBase(sdVoiceIdx),
                           md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
                     }
                     instrumentIt->unmarkAsDefaultCreated();
                  });
         });
   });
}

Void MelodicInstrumentsModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
         return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
            [&,this](auto md) -> Void {
               return MelodicInstrumentModifier(*instrumentIt)
                                      .findComponentIdxToPlaceNewComponent(rFactoryDataHolder, sdUuid, sdVoiceIdx).and_then(
                  [&,this](auto componentIdx) -> Void {
                     if(!mddescrutil::vector_index_in_range(voiceIdx, instrumentIt->m_voices))
                     {
                        return tl::unexpected(Error::indexOutOfRange);
                     }
                     instrumentIt->m_voices.at(voiceIdx).components[componentIdx] =
                         MelodicInstrument::Voice::Component{ md->deviceId(), sdVoiceIdx };
                     if(!instrumentIt->m_parameters[componentIdx].has_value())
                     {
                        instrumentIt->m_parameters[componentIdx] = MelodicInstrument::ParameterData(
                           md->description()->soundSection->engineBase(sdVoiceIdx),
                           md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size());
                     }
                     instrumentIt->unmarkAsDefaultCreated();
                     return Void();
                  });
         });
   });
   // GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   // auto md = rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   // if (md && md->soundHandler)
   // {
   //    auto componentIdx =
   //        MelodicInstrumentsParameterCacheCreator(rFactoryDataHolder)
   //            .findComponentIdxToPlaceNewComponentInVoice(
   //                *instrumentIt, voiceIdx, sdUuid, sdVoiceIdx);
   //    if (componentIdx)
   //    {
   //       instrumentIt->voices().operator[](voiceIdx).components[componentIdx
   //                                                                  .value()] =
   //           Component(&md->soundHandler.value(), std::move(parameterCache),
   //                     md->deviceId(), sdVoiceIdx, 0);
   //       instrumentIt->unmarkAsDefaultCreated();
   //    }
   // }
}
/*
void MelodicInstrumentsModifier::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().at(voiceIdx).components.at(componentIdx).reset();
   instrumentIt->unmarkAsDefaultCreated();
}

void MelodicInstrumentsModifier::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + voiceIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void MelodicInstrumentsModifier::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, 
    int componentIdx, int noteOffset) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->forEachComponent(
       [&noteOffset](Component& component) { 
         component.setNoteOffset(noteOffset); 
      });
   instrumentIt->unmarkAsDefaultCreated();
}

void MelodicInstrumentsModifier::setVoiceNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](voiceIdx).name = name;
   instrumentIt->unmarkAsDefaultCreated();
}


void MelodicInstrumentsModifier::setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                            int componentIdx, float amp)
{
   GET_MELODIC_INSTR_OR_RETURN(uuid);
   instrumentIt->forEachComponent([&amp](Component& component) {
      // TODO component.setAmp(amp, instrumentIt->rtD externalAmp());
   });
}

void MelodicInstrumentsModifier::incMelodicInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->incRefCount();
}
void MelodicInstrumentsModifier::decMelodicInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->decRefCount();
}
*/
// Ret<size_t>
// MelodicInstrumentsModifier::findComponentIdxToPlaceNewComponent(
//     const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
// {
//    auto engineType = determineComponentEngineType(sdUuid, sdVoiceIdx);
//    if (!engineType)
//    {
//       return std::nullopt;
//    }
//    for (size_t componentIdx = 0;
//         componentIdx < MelodicInstrument::NUM_COMPONENTS;
//         ++componentIdx)
//    {
//       auto componentEngineType = getComponentEngineType(componentIdx);
//       if (!componentEngineType)
//       {
//          return componentIdx;
//       }
//       if (componentEngineType.value() == engineType.value())
//       {
//          return componentIdx;
//       }
//    }
//    return std::nullopt;
// }
/*
std::optional<size_t> MelodicInstrumentsModifier::
    findComponentIdxToPlaceNewComponentInVoice(
        int voiceIdx,
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
          getComponentEngineType(componentIdx);
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
*/
