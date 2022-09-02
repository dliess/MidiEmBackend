#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "SoundSection.h"

using namespace base::instruments;

#define GET_MELODIC_INSTR_OR_RETURN(instrumentUuid)                    \
   auto instrumentIt = std::find_if(                                   \
       data.melodicInstruments.begin(), data.melodicInstruments.end(), \
       [&instrumentUuid](const MelodicInstrument& instr) {             \
          return instr.id() == instrumentUuid;                         \
       });                                                             \
   if (instrumentIt == data.melodicInstruments.end())                  \
   {                                                                   \
      return;                                                          \
   }

#define GET_KIT_INSTR_OR_RETURN(instrumentUuid)                             \
   auto instrumentIt =                                                      \
       std::find_if(data.kitInstruments.begin(), data.kitInstruments.end(), \
                    [&instrumentUuid](const KitInstrument& instr) {         \
                       return instr.id() == instrumentUuid;                 \
                    });                                                     \
   if (instrumentIt == data.kitInstruments.end())                           \
   {                                                                        \
      return;                                                               \
   }

Instruments::Instruments(
    musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

Instruments::Settings Instruments::getSettings() const noexcept { return data; }

void Instruments::setSettings(const Settings& settings) noexcept
{
   data = settings;
}

void Instruments::registerForDataChange(Cb cb) noexcept
{
   m_subscribers.push_back(cb);
}

void Instruments::triggerChanged() noexcept
{
   for (auto& cb : m_subscribers) cb();
}

void Instruments::createKitInstrument(std::string name) noexcept
{
   data.kitInstruments.emplace_back(std::move(name));
   triggerChanged();
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentId);
   data.kitInstruments.erase(instrumentIt);
   triggerChanged();
}

void Instruments::createMelodicInstrument(std::string name) noexcept
{
   data.melodicInstruments.emplace_back(std::move(name));
   triggerChanged();
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentId);
   data.melodicInstruments.erase(instrumentIt);
   triggerChanged();
}

void Instruments::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto musicDeviceIt = m_rMusicDeviceContainer.find(soundDeviceUuid);
   if (musicDeviceIt == m_rMusicDeviceContainer.end())
   {
      return;
   }
   auto& sh = musicDeviceIt->second->soundHandler;
   if (!sh)
   {
      return;
   }
   const auto& ss = musicDeviceIt->second->description()->soundSection;
   CompositeSound compositeSound(ss->voices[voiceIdx].name);
   compositeSound.voices.push_back(
       Voice{&sh.value(), musicDeviceIt->second->deviceId(), voiceIdx, 0});
   instrumentIt->voices().push_back(compositeSound);
   triggerChanged();
}

void Instruments::addVoiceToMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   auto musicDeviceIt = m_rMusicDeviceContainer.find(soundDeviceUuid);
   if (musicDeviceIt == m_rMusicDeviceContainer.end())
   {
      return;
   }
   auto& sh = musicDeviceIt->second->soundHandler;
   if (!sh)
   {
      return;
   }
   instrumentIt->voices().operator[](slotIdx).voices.push_back(
       Voice{&sh.value(), musicDeviceIt->second->deviceId(), voiceIdx, 0});
   triggerChanged();
}

void Instruments::removeVoiceFromMelodicInstrumentSlot(
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
   triggerChanged();
}

void Instruments::removeSlotFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().erase(instrumentIt->voices().begin() + slotIdx);
   triggerChanged();
}

void Instruments::setNoteOffsetInMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](slotIdx).voices[compositeIdx].noteOffset =
       noteOffset;
   triggerChanged();
}

void Instruments::setCompositeNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string name) noexcept
{
   GET_MELODIC_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->voices().operator[](slotIdx).name = name;
   triggerChanged();
}

void Instruments::createNewSlotInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto musicDeviceIt = m_rMusicDeviceContainer.find(soundDeviceUuid);
   if (musicDeviceIt == m_rMusicDeviceContainer.end())
   {
      spdlog::error("Could not find md: {}", util::uuid2Str(soundDeviceUuid));
      return;
   }
   auto& sh = musicDeviceIt->second->soundHandler;
   if (!sh)
   {
      return;
   }
   const auto& ss = musicDeviceIt->second->description()->soundSection;
   CompositeSound compositeSound(ss->voices[voiceIdx].name);
   compositeSound.voices.push_back(
       Voice{&sh.value(), musicDeviceIt->second->deviceId(), voiceIdx, 0});
   instrumentIt->sounds().push_back(compositeSound);
   triggerChanged();
}

void Instruments::addVoiceToKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto musicDeviceIt = m_rMusicDeviceContainer.find(soundDeviceUuid);
   if (musicDeviceIt == m_rMusicDeviceContainer.end())
   {
      spdlog::error("Could not find md: {}", util::uuid2Str(soundDeviceUuid));
      return;
   }
   auto& sh = musicDeviceIt->second->soundHandler;
   if (!sh)
   {
      return;
   }
   if (instrumentIt->sounds().operator[](slotIdx).voices.size() >=
       MAX_VOICES_IN_SLOT)
   {
      return;
   }
   instrumentIt->sounds().operator[](slotIdx).voices.push_back(
       Voice{&sh.value(), musicDeviceIt->second->deviceId(), voiceIdx, 0});
   triggerChanged();
}

void Instruments::moveKitInstrumentSlotVoice(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
    int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx) noexcept
{
   KitInstruments::Super::iterator srcInstrumentIt;
   {
      GET_KIT_INSTR_OR_RETURN(srcInstrumentUuid);
      srcInstrumentIt = instrumentIt;
   }
   KitInstruments::Super::iterator dstInstrumentIt;
   {
      GET_KIT_INSTR_OR_RETURN(dstInstrumentUuid);
      dstInstrumentIt = instrumentIt;
   }
   const auto& srcVoice = srcInstrumentIt->sounds().operator[](srcSlotIdx).voices[srcCompositeIdx];
   dstInstrumentIt->sounds().operator[](dstSlotIdx).voices.push_back(srcVoice);
   removeVoiceFromKitInstrumentSlot(srcInstrumentUuid, srcSlotIdx, srcCompositeIdx);
}

void Instruments::removeVoiceFromKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   auto& voices = instrumentIt->sounds().operator[](slotIdx).voices;
   voices.erase(voices.begin() + compositeIdx);
   triggerChanged();
}

void Instruments::removeSlotFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds().erase(instrumentIt->sounds().begin() + slotIdx);
   triggerChanged();
}

void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds().operator[](slotIdx).voices[compositeIdx].noteOffset =
       noteOffset;
   triggerChanged();
}

void Instruments::setCompositeNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name) noexcept
{
   GET_KIT_INSTR_OR_RETURN(instrumentUuid);
   instrumentIt->sounds().operator[](slotIdx).name = name;
   triggerChanged();
}