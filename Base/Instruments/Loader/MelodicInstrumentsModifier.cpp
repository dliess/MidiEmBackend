#include "MelodicInstrumentsModifier.h"

#include <spdlog/spdlog.h>


using namespace base::instruments::loader;

#define GET_MELODIC_INSTR_OR_RETURN(instrumentUuid)                          \
   auto instrumentIt = std::ranges::find_if(                                 \
       m_rMelodicInstruments,                                                \
       [&instrumentUuid](const MelodicInstrument& instr) {                   \
          return instr.id() == instrumentUuid;                               \
       });                                                                   \
   if (instrumentIt == m_rMelodicInstruments.end())                          \
   {                                                                         \
      return;                                                                \
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

void MelodicInstrumentsModifier::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentId);
   m_rMelodicInstruments.erase(instrumentIt);
}

void MelodicInstrumentsModifier::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentId);
   instrumentIt->setName(std::move(name));
   instrumentIt->unmarkAsDefaultCreated();
}

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
