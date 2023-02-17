#include "InstrumentsModifier.h"

#include <spdlog/spdlog.h>

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
}

void InstrumentsModifier::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
    std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(soundDeviceUuid);
   if (md && md->soundHandler)
   {
      CompositeSound compositeSound(
          md->description()->soundSection->voices[voiceIdx].name);
      compositeSound.voices.emplace_back(&md->soundHandler.value(),
                                         std::move(parameterCache),
                                         md->deviceId(), voiceIdx, 0);
      instrumentIt->voices().push_back(std::move(compositeSound));
   }
}

void InstrumentsModifier::addVoiceToMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
    std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(soundDeviceUuid);
   if (md && md->soundHandler)
   {
      instrumentIt->voices().operator[](slotIdx).voices.emplace_back(
          &md->soundHandler.value(), std::move(parameterCache), md->deviceId(),
          voiceIdx, 0);
   }
}

void InstrumentsModifier::removeVoiceFromMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto& voices = instrumentIt->voices().operator[](slotIdx).voices;
   voices.erase(voices.begin() + compositeIdx);
   if (voices.size() == 0)
   {
      instrumentIt->voices().erase(instrumentIt->voices().begin() + slotIdx);
   }
}

void InstrumentsModifier::removeSlotFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + slotIdx);
}

void InstrumentsModifier::setNoteOffsetInMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices()
       .
       operator[](slotIdx)
       .voices[compositeIdx]
       .setNoteOffset(noteOffset);
}

void InstrumentsModifier::setCompositeNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](slotIdx).name = name;
}

void InstrumentsModifier::createNewSlotInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
    std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(soundDeviceUuid);
   if (md && md->soundHandler)
   {
      CompositeSound compositeSound(
          md->description()->soundSection->voices[voiceIdx].name);
      compositeSound.voices.emplace_back(&md->soundHandler.value(),
                                         std::move(parameterCache),
                                         md->deviceId(), voiceIdx, 0);
      instrumentIt->sounds().push_back(std::move(compositeSound));
   }
}

void InstrumentsModifier::addVoiceToKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
    std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = m_rFactoryDataHolder.getMusicDeviceByUUID(soundDeviceUuid);
   if (md && md->soundHandler)
   {
      if (instrumentIt->sounds().operator[](slotIdx).voices.size() >=
          MAX_VOICES_IN_SLOT)
      {
         return;
      }
      instrumentIt->sounds().operator[](slotIdx).voices.emplace_back(
          &md->soundHandler.value(), std::move(parameterCache), md->deviceId(),
          voiceIdx, 0);
   }
}

void InstrumentsModifier::moveKitInstrumentSlotVoice(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
    int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx) noexcept
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
   const auto& srcVoice =
       srcInstrumentIt->sounds().operator[](srcSlotIdx).voices[srcCompositeIdx];
   dstInstrumentIt->sounds().operator[](dstSlotIdx).voices.push_back(srcVoice);
   removeVoiceFromKitInstrumentSlot(srcInstrumentUuid, srcSlotIdx,
                                    srcCompositeIdx);
}

void InstrumentsModifier::removeVoiceFromKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto& voices = instrumentIt->sounds().operator[](slotIdx).voices;
   voices.erase(voices.begin() + compositeIdx);
}

void InstrumentsModifier::removeSlotFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds().erase(instrumentIt->sounds().begin() + slotIdx);
}

void InstrumentsModifier::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds()
       .
       operator[](slotIdx)
       .voices[compositeIdx]
       .setNoteOffset(noteOffset);
}

void InstrumentsModifier::setCompositeNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds().operator[](slotIdx).name = name;
}

std::shared_ptr<Voice::ParameterCache>
InstrumentsModifier::createParameterCache(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto descr = rFactoryDataHolder.getDescription(soundDeviceUuid);
   if (descr && descr->soundSection)
   {
      const auto engine = descr->soundSection->engineBase(voiceIdx);
      if (engine)
      {
         auto paramCache =
             std::make_shared<Voice::ParameterCache>(engine->parameters.size());
         for (int paramIdx = 0; paramIdx < paramCache->size(); ++paramIdx)
         {
            paramCache->at(paramIdx).commanded =
                descr->soundSection->getInitialValueFor(voiceIdx, paramIdx);
         }
         return paramCache;
      }
   }
   return nullptr;
}