#include "KitInstrumentsModifier.h"

#include <spdlog/spdlog.h>

using namespace base::instruments::loader;

#define GET_KIT_INSTR_OR_RETURN(instrumentUuid)                      \
   auto instrumentIt = std::ranges::find_if(                         \
       m_rKitInstruments,                                            \
       [&instrumentUuid](const KitInstrument& instr) {               \
          return instr.id() == instrumentUuid;                       \
       });                                                           \
   if (instrumentIt == m_rKitInstruments.end())                      \
   {                                                                 \
      return;                                                        \
   }

KitInstrumentsModifier::KitInstrumentsModifier(KitInstruments& rKitInstruments) noexcept :
    m_rKitInstruments(rKitInstruments)
{
}

void KitInstrumentsModifier::insertKitInstrument(
    KitInstrument kitInstrument) noexcept
{
   m_rKitInstruments.push_back(std::move(kitInstrument));
}

void KitInstrumentsModifier::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentId);
   m_rKitInstruments.erase(instrumentIt);
}

void KitInstrumentsModifier::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentId);
   instrumentIt->setName(std::move(name));
   instrumentIt->unmarkAsDefaultCreated();
}

void KitInstrumentsModifier::createNewVoiceInKitInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md.value()->soundHandler)
   {
      KitVoice voice(md.value()->description()->soundSection->voices[sdVoiceIdx].name);
      voice.components.emplace_back(&md.value()->soundHandler.value(),
                                    md.value()->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
                                    md.value()->deviceId(),
                                    sdVoiceIdx, 0);
      instrumentIt->voices().push_back(std::move(voice));
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void KitInstrumentsModifier::addComponentToKitInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto md = rFactoryDataHolder.getMusicDeviceByUUID(sdUuid);
   if (md && md.value()->soundHandler)
   {
      if (instrumentIt->voices().operator[](voiceIdx).components.size() >=
          KitVoice::NUM_MAX_COMPONENTS_PER_VOICE)
      {
         return;
      }
      instrumentIt->voices().operator[](voiceIdx).components.emplace_back(
          &md.value()->soundHandler.value(),
          md.value()->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
          md.value()->deviceId(),
          sdVoiceIdx, 0);
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void KitInstrumentsModifier::moveKitInstrumentComponent(
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

void KitInstrumentsModifier::removeComponentFromKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto& components = instrumentIt->voices().at(voiceIdx).components;
   components.erase(components.begin() + componentIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void KitInstrumentsModifier::removeVoiceFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + voiceIdx);
   instrumentIt->unmarkAsDefaultCreated();
}

void KitInstrumentsModifier::setNoteOffsetInKitInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().at(voiceIdx)
       .components[componentIdx]
       .setNoteOffset(noteOffset);
   instrumentIt->unmarkAsDefaultCreated();
}

void KitInstrumentsModifier::setNoteOffsetInKitInstrumentVoice(const util::Identifiable::UUID& instrumentUuid,
                                       int voiceIdx, int noteOffset) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   if (noteOffset != instrumentIt->voices().at(voiceIdx).noteOffset)
   {
      instrumentIt->voices().at(voiceIdx).noteOffset = noteOffset;
      instrumentIt->unmarkAsDefaultCreated();
   }
}

void KitInstrumentsModifier::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](voiceIdx).name = name;
   instrumentIt->unmarkAsDefaultCreated();
}
void KitInstrumentsModifier::setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                        int componentIdx, float amp)
{
   GET_KIT_INSTR_OR_RETURN(uuid);
   instrumentIt->setComponentAmp(voiceIdx, componentIdx, amp);
}

void KitInstrumentsModifier::setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp)
{
   GET_KIT_INSTR_OR_RETURN(uuid);
   instrumentIt->setVoiceAmp(voiceIdx, amp);
}

void KitInstrumentsModifier::incKitInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->incRefCount();
}
void KitInstrumentsModifier::decKitInstrumentRefCount(
    const util::Identifiable::UUID& instrumentUuid)
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->decRefCount();
}
