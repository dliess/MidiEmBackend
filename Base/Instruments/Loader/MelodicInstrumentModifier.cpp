#include "MelodicInstrumentModifier.h"

#include <spdlog/spdlog.h>
#include "Identifiable.h"
#include "MelodicInstrument.h"


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


/*
void MelodicInstrumentsModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      MelodicVoice voice(
          md->description()->soundSection->voices[sdVoiceIdx].name);
      auto componentIdx =
          MelodicInstrumentsParameterCacheCreator(rFactoryDataHolder)
              .findComponentIdxToPlaceNewComponent(*instrumentIt, sdUuid,
                                                   sdVoiceIdx);
      if (componentIdx)
      {
         voice.components[componentIdx.value()] =
             Component(&md->soundHandler.value(), std::move(parameterCache),
                       md->deviceId(), sdVoiceIdx, 0);
      }
      instrumentIt->voices().push_back(std::move(voice));
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void MelodicInstrumentsModifier::addComponentToMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      auto componentIdx =
          MelodicInstrumentsParameterCacheCreator(rFactoryDataHolder)
              .findComponentIdxToPlaceNewComponentInVoice(
                  *instrumentIt, voiceIdx, sdUuid, sdVoiceIdx);
      if (componentIdx)
      {
         instrumentIt->voices().operator[](voiceIdx).components[componentIdx
                                                                    .value()] =
             Component(&md->soundHandler.value(), std::move(parameterCache),
                       md->deviceId(), sdVoiceIdx, 0);
         instrumentIt->unmarkAsDefaultCreated();
      }
   }
}

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

std::optional<size_t>
MelodicInstrumentsModifier::findComponentIdxToPlaceNewComponent(
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

std::optional<std::pair<musicDevice::MusicDeviceName, int>>
MelodicInstrumentsModifier::determineComponentEngineType(
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

std::optional<std::pair<musicDevice::MusicDeviceName, int>>
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
   return std::make_pair(
       mdName, descr->soundSection->voice2EngineIdx(pComponent->m_sdVoiceIdx));
}

*/
Ret<size_t>
MelodicInstrumentsModifier::findComponentIdxToPlaceNewComponent(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   auto enginePtr = rFactoryDataHolder.getMdIdByUUID(sdUuid), sdVoiceIdx);
   std::ranges::find_if(m_rMelodicInstrument.m_parameters, [](auto& parameterData){
      return 
   });
   
   });
   auto engineType = determineComponentEngineType(sdUuid, sdVoiceIdx);
   if (!engineType)
   {
      return std::nullopt;
   }
   for (size_t componentIdx = 0;
        componentIdx < MelodicInstrument::NUM_COMPONENTS;
        ++componentIdx)
   {
      auto componentEngineType = getComponentEngineType(componentIdx);
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

Ret<musicDevice::description::sound::Engine*>
MelodicInstrumentsModifier::determineComponentEngineType(
   base::musicDevice::factory::DataHolder& rFactoryDataHolder,
   const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const
{
   const auto descr = rFactoryDataHolder.getDescription(sdUuid);
   if (!descr)
   {
      return tl::unexpected(Error::mdDescrNotFound);
   }
   descr->soundSection->
   return std::make_pair(musicDeviceId->deviceName(),
                         descr->soundSection->voice2EngineIdx(sdVoiceIdx));
}


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

