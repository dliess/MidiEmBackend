#include "InstrumentsModifier.h"

#include <spdlog/spdlog.h>

#include "InstrumentComponentParameterCacheCreator.h"

using namespace base::instruments;

#define GET_MELODIC_INSTR_OR_RETURN(instrumentUuid)                          \
   auto instrumentIt = std::find_if(                                         \
       m_rData.melodicInstruments.begin(), m_rData.melodicInstruments.end(), \
       [&instrumentUuid](const MelodicInstrument& instr) {                   \
          return instr.id() == instrumentUuid;                               \
       });                                                                   \
   if (instrumentIt == m_rData.melodicInstruments.end())                     \
   {                                                                         \
      return;                                                                \
   }

#define GET_KIT_INSTR_OR_RETURN(instrumentUuid)                      \
   auto instrumentIt = std::find_if(                                 \
       m_rData.kitInstruments.begin(), m_rData.kitInstruments.end(), \
       [&instrumentUuid](const KitInstrument& instr) {               \
          return instr.id() == instrumentUuid;                       \
       });                                                           \
   if (instrumentIt == m_rData.kitInstruments.end())                 \
   {                                                                 \
      return;                                                        \
   }

InstrumentsModifier::InstrumentsModifier(
    Data& rData,
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept :
    m_rData(rData), m_rFactoryDataHolder(rFactoryDataHolder)
{
}

void InstrumentsModifier::insertKitInstrument(
    KitInstrument kitInstrument) noexcept
{
   m_rData.kitInstruments.push_back(std::move(kitInstrument));
}

void InstrumentsModifier::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentId);
   m_rData.kitInstruments.erase(instrumentIt);
}

void InstrumentsModifier::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentId);
   instrumentIt->setName(std::move(name));
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::insertMelodicInstrument(
    MelodicInstrument melodicInstrument) noexcept
{
   m_rData.melodicInstruments.push_back(std::move(melodicInstrument));
}

void InstrumentsModifier::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentId);
   m_rData.melodicInstruments.erase(instrumentIt);
}

void InstrumentsModifier::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentId);
   instrumentIt->setName(std::move(name));
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::createNewVoiceInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    std::shared_ptr<ParameterCache> parameterCache) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      MelodicVoice voice(
          md->description()->soundSection->voices[sdVoiceIdx].name);
      auto componentIdx =
          MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
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

void InstrumentsModifier::addComponentToMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    std::shared_ptr<ParameterCache> parameterCache) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      auto componentIdx =
          MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
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

void InstrumentsModifier::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().at(voiceIdx).components.at(componentIdx).reset();
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + voiceIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto& component =
       instrumentIt->voices().at(voiceIdx).components[componentIdx];
   if (component)
   {
      component->setNoteOffset(noteOffset);
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void InstrumentsModifier::setVoiceNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](voiceIdx).name = name;
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::createNewVoiceInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    std::shared_ptr<ParameterCache> parameterCache) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      KitVoice voice(md->description()->soundSection->voices[sdVoiceIdx].name);
      voice.components.emplace_back(&md->soundHandler.value(),
                                    std::move(parameterCache), md->deviceId(),
                                    sdVoiceIdx, 0);
      instrumentIt->voices().push_back(std::move(voice));
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void InstrumentsModifier::addComponentToKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
    std::shared_ptr<ParameterCache> parameterCache) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md->soundHandler)
   {
      if (instrumentIt->voices().operator[](voiceIdx).components.size() >=
          KitVoice::NUM_MAX_COMPONENTS_PER_VOICE)
      {
         return;
      }
      instrumentIt->voices().operator[](voiceIdx).components.emplace_back(
          &md->soundHandler.value(), std::move(parameterCache), md->deviceId(),
          sdVoiceIdx, 0);
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void InstrumentsModifier::moveKitInstrumentComponent(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
    int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstVoiceIdx) noexcept
{
   KitInstruments::iterator srcInstrumentIt;
   {
      GET_KIT_INSTR_OR_RETURN(srcInstrumentUuid);
      srcInstrumentIt = instrumentIt;
   }
   KitInstruments::iterator dstInstrumentIt;
   {
      GET_KIT_INSTR_OR_RETURN(dstInstrumentUuid);
      dstInstrumentIt = instrumentIt;
   }
   const auto& srcVoice = srcInstrumentIt->voices()
                              .
                              operator[](srcVoiceIdx)
                              .components[srcComponentIdx];
   dstInstrumentIt->voices()
       .
       operator[](dstVoiceIdx)
       .components.push_back(srcVoice);
   removeComponentFromKitInstrumentVoice(srcInstrumentUuid, srcVoiceIdx,
                                         srcComponentIdx);
   srcInstrumentIt->unmarkAsDefaultCreated();
   dstInstrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::removeComponentFromKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto& components = instrumentIt->voices().at(voiceIdx).components;
   components.erase(components.begin() + componentIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::removeVoiceFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + voiceIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::setNoteOffsetInKitInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().at(voiceIdx)
       .components[componentIdx]
       .setNoteOffset(noteOffset);
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](voiceIdx).name = name;
   instrumentIt->unmarkAsDefaultCreated();
}

void InstrumentsModifier::incKitInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->incRefCount();
}
void InstrumentsModifier::decKitInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->decRefCount();
}
void InstrumentsModifier::incMelodicInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->incRefCount();
}
void InstrumentsModifier::decMelodicInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->decRefCount();
}
