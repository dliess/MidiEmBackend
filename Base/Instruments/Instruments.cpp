#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "FilePersister.h"
#include "InstrumentVoiceFactory.h"
#include "InstrumentsMDRefSetter.h"
#include "InstrumentsModifier.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "SoundSection.h"

using namespace base;
using namespace base::instruments;

Instruments::Instruments(
    musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept :
    m_rFactoryDataHolder(rFactoryDataHolder),
    m_persister(
        std::make_unique<util::FilePersister>("Instruments", "settings.json"),
        rFactoryDataHolder)
{
   onDataChanged([this](const instruments::Data& data, bool doSaveToFile) {
      if (doSaveToFile)
      {
         m_persister.save(data);
      }
   });

   try
   {
      auto data = m_persister.load();
      m_doubleBufferedData.withNonRtLocked(
          [&data](auto& nonRtData) { nonRtData = data; });
   }
   catch (std::exception& e)
   {
      spdlog::error(
          "Error loading Instruments settings, its maybe the first run: {}",
          e.what());
   }
}

void Instruments::createKitInstrument(std::string name)
{
   KitInstrument kitInstrument(std::move(name));
   m_doubleBufferedData.withNonRtLocked(
       [this, &kitInstrument](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .insertKitInstrument(kitInstrument);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::insertKitInstrument(const KitInstrument& kitInstrument)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &kitInstrument](auto& nonRtData) {
          nonRtData.kitInstruments.push_back(kitInstrument);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

bool Instruments::hasKitInstrument(const KitInstrument& kitInstrument) const
{
   const auto& data = m_doubleBufferedData.nonRt();
   for (const auto& e : data.kitInstruments)
   {
      if (isSameInstrument(kitInstrument, e))
      {
         return true;
      }
   }
   return false;
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentId](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeKitInstrument(instrumentId);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentId, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .renameKitInstrument(instrumentId, name);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::createMelodicInstrument(std::string name)
{
   MelodicInstrument melodicInstrument(
       std::move(name), std::make_shared<MelodicInstrument::RtData>());
   m_doubleBufferedData.withNonRtLocked(
       [this, &melodicInstrument](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .insertMelodicInstrument(melodicInstrument);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

bool Instruments::hasMelodicInstrument(
    const MelodicInstrument& melodicInstrument) const
{
   const auto& data = m_doubleBufferedData.nonRt();
   for (const auto& e : data.melodicInstruments)
   {
      if (isSameInstrument(melodicInstrument, e))
      {
         return true;
      }
   }
   return false;
}

void Instruments::insertMelodicInstrument(
    const MelodicInstrument& melodicInstrument)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &melodicInstrument](auto& nonRtData) {
          nonRtData.melodicInstruments.push_back(melodicInstrument);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentId](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeMelodicInstrument(instrumentId);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentId, name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .renameMelodicInstrument(instrumentId, name);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = createParameterCache(
       m_rFactoryDataHolder.getDescription(soundDeviceUuid), voiceIdx);
    /*
   paramCache->onDataChangedUI(
       [instrumentUuid, voiceIdx, this](int parameterId,
                 musicDevice::sound::ParameterAttr parameterAttr, float value) {
          emitMusicInstrumentParamChanged(instrumentUuid, voiceIdx, parameterId,
                                          parameterAttr, value);
       });
       */
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid,
                                         &soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewSlotInMelodicInstrument(instrumentUuid, soundDeviceUuid,
                                            voiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::addVoiceToMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = createParameterCache(
       m_rFactoryDataHolder.getDescription(soundDeviceUuid), voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   /*
   paramCache->onDataChangedUI(
       [instrumentUuid, voiceIdx, this](int parameterId,
                 musicDevice::sound::ParameterAttr parameterAttr, float value) {
          emitMusicInstrumentParamChanged(instrumentUuid, voiceIdx, parameterId,
                                          parameterAttr, value);
       });
       */
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         &soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addVoiceToMelodicInstrumentSlot(
              instrumentUuid, slotIdx, soundDeviceUuid, voiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeVoiceFromMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, compositeIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeVoiceFromMelodicInstrumentSlot(instrumentUuid, slotIdx,
                                                    compositeIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeSlotFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeSlotFromMelodicInstrument(instrumentUuid, slotIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setNoteOffsetInMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         compositeIdx,
                                         noteOffset](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInMelodicInstrumentVoice(instrumentUuid, slotIdx,
                                                 compositeIdx, noteOffset);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setCompositeNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         &name](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setCompositeNameInMelodicInstrument(instrumentUuid, slotIdx, name);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::createNewSlotInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = createParameterCache(
       m_rFactoryDataHolder.getDescription(soundDeviceUuid), voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   /*
   paramCache->onDataChangedUI(
       [instrumentUuid, voiceIdx, this](int parameterId,
                 musicDevice::sound::ParameterAttr parameterAttr, float value) {
          emitKitInstrumentParamChanged(instrumentUuid, voiceIdx, parameterId,
                                          parameterAttr, value);
       });
       */
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, soundDeviceUuid,
                                         voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewSlotInKitInstrument(instrumentUuid, soundDeviceUuid,
                                        voiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::addVoiceToKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = createParameterCache(
       m_rFactoryDataHolder.getDescription(soundDeviceUuid), voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   /*
   paramCache->onDataChangedUI(
       [instrumentUuid, slotIdx, this](int parameterId,
                 musicDevice::sound::ParameterAttr parameterAttr, float value) {
          emitKitInstrumentParamChanged(instrumentUuid, slotIdx, parameterId,
                                          parameterAttr, value);
       });
       */
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addVoiceToKitInstrumentSlot(instrumentUuid, slotIdx, soundDeviceUuid,
                                       voiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::moveKitInstrumentSlotVoice(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
    int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .moveKitInstrumentSlotVoice(srcInstrumentUuid, srcSlotIdx,
                                      srcCompositeIdx, dstInstrumentUuid,
                                      dstSlotIdx);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeVoiceFromKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, compositeIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeVoiceFromKitInstrumentSlot(instrumentUuid, slotIdx,
                                                compositeIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeSlotFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeSlotFromKitInstrument(instrumentUuid, slotIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         compositeIdx,
                                         noteOffset](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInKitInstrumentVoice(instrumentUuid, slotIdx,
                                             compositeIdx, noteOffset);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setCompositeNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .setCompositeNameInKitInstrument(instrumentUuid, slotIdx, name);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::incKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .incKitInstrumentRefCount(uuid);
   });
}
void Instruments::decKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .decKitInstrumentRefCount(uuid);
   });
}
void Instruments::incMelodicInstrumentRefCount(
    const util::Identifiable::UUID& uuid)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .incMelodicInstrumentRefCount(uuid);
   });
}
void Instruments::decMelodicInstrumentRefCount(
    const util::Identifiable::UUID& uuid)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .decMelodicInstrumentRefCount(uuid);
   });
}

template <typename Container>
auto elementWithUuid(Container& container,
                     util::Identifiable::UUIDView uuidView)
{
   return std::find_if(container.begin(), container.end(),
                       [uuidView](const auto& element) {
                          return element.idView() == uuidView;
                       });
}

const Instrument* Instruments::getInstrumentByUuid(
    util::Identifiable::UUIDView uuid)
{
   auto it1 =
       elementWithUuid(m_doubleBufferedData.nonRt().kitInstruments, uuid);
   if (it1 != m_doubleBufferedData.nonRt().kitInstruments.end())
   {
      return &(*it1);
   }
   auto it2 =
       elementWithUuid(m_doubleBufferedData.nonRt().melodicInstruments, uuid);
   if (it2 != m_doubleBufferedData.nonRt().melodicInstruments.end())
   {
      return &(*it2);
   }
   return nullptr;
}

std::string Instruments::serializeKitInstruments() const
{
   return nlohmann::json(m_doubleBufferedData.nonRt().kitInstruments)
       .dump()
       .c_str();
}

std::string Instruments::serializeMelodicInstruments() const
{
   return nlohmann::json(m_doubleBufferedData.nonRt().melodicInstruments)
       .dump()
       .c_str();
}

void Instruments::fillReferencesToMD(musicDevice::MusicDevice* pMusicDevice)
{
   m_doubleBufferedData.withNonRtLocked([this, pMusicDevice](auto& nonRtData) {
      InstrumentsMDRefSetter(nonRtData).fillReferencesKitInstruments(
          pMusicDevice);
      InstrumentsMDRefSetter(nonRtData).fillReferencesMelodicInstruments(
          pMusicDevice);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), false);
}

void Instruments::removeReferencesToMD(musicDevice::MusicDevice* pMusicDevice)
{
   m_doubleBufferedData.withNonRtLocked([this, pMusicDevice](auto& nonRtData) {
      InstrumentsMDRefSetter(nonRtData).removeKitInstruments(pMusicDevice);
      InstrumentsMDRefSetter(nonRtData).removeMelodicInstruments(pMusicDevice);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), false);
}

void Instruments::reEmitSignals()
{
   emitDataChanged(m_doubleBufferedData.nonRt(), false);
}