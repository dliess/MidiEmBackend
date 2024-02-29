#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "FilePersister.h"
#include "Loader/KitInstrumentsModifier.h"
#include "Loader/MelodicInstrumentsModifier.h"
#include "Rt/KitInstrumentsModifier.h"
#include "Rt/MelodicInstrumentsModifier.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "SoundSection.h"
#include "FixedSizeString.h"
#include "KitInstrumentCopyer.h"
#include "MusicDeviceContainer.h"

using namespace base;
using namespace base::instruments;

Instruments::Instruments(
    musicDevice::factory::DataHolder& rFactoryDataHolder,
    musicDevice::MusicDeviceContainer& rMDContainer) noexcept :
    m_rFactoryDataHolder(rFactoryDataHolder),
    m_persister(
        std::make_unique<util::FilePersister>("Instruments", "settings.json"),
        rFactoryDataHolder)
{
   /*
   onDataChanged([this](const instruments::Data& data, bool doSaveToFile) {
      if (doSaveToFile)
      {
         m_persister.save(data);
         m_parameterCacheDirty = false;
      }
   });

   try
   {
      auto data = m_persister.load();
      for (auto& instr : data.kitInstruments)
      {
         KitInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
             .initParameterCaches(instr, *this);
         spdlog::info("Loaded KitInstrument with uuid: {}", util::uuid2Str(instr.id()));
      }
      for (auto& instr : data.melodicInstruments)
      {
         MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
             .initParameterCaches(instr, *this);
         spdlog::info("Loaded MelodicInstrument with uuid: {}", util::uuid2Str(instr.id()));
      }
      m_doubleBufferedData.withNonRtLocked(
          [&data](auto& nonRtData) { nonRtData = data; });
   }
   catch (std::exception& e)
   {
      spdlog::error(
          "Error loading Instruments settings, its maybe the first run: {}",
          e.what());
   }
*/
}

void Instruments::createKitInstrument(std::string name)
{
   auto uuid = loader::KitInstrumentsModifier(m_loaderData.kitInstruments).createKitInstrument(
       std::move(name));
   static constexpr size_t MaxStringSize = 64;
   m_deferToRt.callAsync([this, uuid, 
                           fsName = util::FixedSizeString<MaxStringSize>(name)]() {
                              m_rtData.kitInstruments.emplace_back(uuid, fsName);
                           });
   emitDataChanged(m_loaderData, true);
}

void Instruments::insertKitInstrument(loader::KitInstrument& kitInstrument)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .insertKitInstrument(kitInstrument);
   m_deferToRt.callAsync([this, copy = std::make_unique<loader::KitInstrument>(kitInstrument)]() mutable { 
      m_rtData.kitInstruments.push_back(KitInstrumentCopyer::copy(*copy));
      m_deferToLoader.callAsync([c = std::move(copy)]() mutable { c.reset(); });
   });
   emitDataChanged(m_loaderData, true);
}

bool Instruments::hasSameInstrument(const loader::KitInstrument& kitInstrument) const
{
   // for (const auto& e : m_loaderData.kitInstruments)
   // {
   //    if (isSameInstrument(kitInstrument, e))
   //    {
   //       return true;
   //    }
   // }:
   return false;
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).removeKitInstrument(instrumentId);
   m_deferToRt.callAsync([this, instrumentId]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments).removeKitInstrument(instrumentId);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .renameKitInstrument(instrumentId, name);
   m_deferToRt.callAsync([this, instrumentId, fsName = util::FixedSizeString<64>(name)]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments).renameKitInstrument(instrumentId, fsName);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::createMelodicInstrument(std::string name)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .createMelodicInstrument(std::move(name));
   emitDataChanged(m_loaderData, true);
}

bool Instruments::hasSameInstrument(
    const loader::MelodicInstrument& melodicInstrument) const
{
   for (const auto& e : m_loaderData.melodicInstruments)
   {
      if (isSameInstrument(melodicInstrument, e))
      {
         return true;
      }
   }
   return false;
}

void Instruments::insertMelodicInstrument(loader::MelodicInstrument& melodicInstrument)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .insertMelodicInstrument(melodicInstrument);
   emitDataChanged(m_loaderData, true);
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeMelodicInstrument(instrumentId);
   emitDataChanged(m_loaderData, true);
}

void Instruments::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .renameMelodicInstrument(instrumentId, name);
   emitDataChanged(m_loaderData, true);
}

void Instruments::createNewVoiceInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .createNewVoiceInMelodicInstrument(m_rFactoryDataHolder, instrumentUuid, sdUuid, sdVoiceIdx);
   emitDataChanged(m_loaderData, true);
}

void Instruments::addComponentToMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .addComponentToMelodicInstrumentVoice(m_rFactoryDataHolder, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx);
   emitDataChanged(m_loaderData, true);
}

void Instruments::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeComponentFromMelodicInstrumentVoice(instrumentUuid, voiceIdx, componentIdx);
   emitDataChanged(m_loaderData, true);
}

void Instruments::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeVoiceFromMelodicInstrument(instrumentUuid, voiceIdx);
   emitDataChanged(m_loaderData, true);
}

void Instruments::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid,
    int componentIdx, int noteOffset)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .setNoteOffsetInMelodicInstrumentComponent(instrumentUuid, componentIdx, noteOffset);
   emitMelodicComponentNoteOffsetChanged(instrumentUuid, componentIdx, noteOffset); // TODO: emit from here or connect to signal?
}

void Instruments::setNoteOffsetInKitInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .setNoteOffsetInKitInstrumentComponent(instrumentUuid, voiceIdx, componentIdx, noteOffset);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx, componentIdx, noteOffset]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .setNoteOffsetInKitInstrumentComponent(instrumentUuid, voiceIdx, componentIdx, noteOffset);
   });
   emitKitComponentNoteOffsetChanged(instrumentUuid, voiceIdx, componentIdx,
                                     noteOffset);  
}
void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int noteOffset)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .setNoteOffsetInKitInstrumentVoice(instrumentUuid, voiceIdx, noteOffset);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx, noteOffset]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .setNoteOffsetInKitInstrumentVoice(instrumentUuid, voiceIdx, noteOffset);
   });
   emitKitVoiceNoteOffsetChanged(instrumentUuid, voiceIdx, noteOffset);
}

void Instruments::createNewVoiceInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .createNewVoiceInKitInstrument(m_rFactoryDataHolder, instrumentUuid, sdUuid, sdVoiceIdx);
   m_deferToRt.callAsync([this, instrumentUuid, sdUuid, sdVoiceIdx]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .createNewVoiceInKitInstrument(m_rMDContainer, instrumentUuid, sdUuid, sdVoiceIdx);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::addComponentToKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .addComponentToKitInstrumentVoice(m_rFactoryDataHolder, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .addComponentToKitInstrumentVoice(m_rMDContainer, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::moveKitInstrumentComponent(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
    int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .moveKitInstrumentComponent(srcInstrumentUuid, srcVoiceIdx, srcComponentIdx,
                                  dstInstrumentUuid, dstSlotIdx);
   m_deferToRt.callAsync([this, srcInstrumentUuid, srcVoiceIdx, srcComponentIdx,
                          dstInstrumentUuid, dstSlotIdx]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .moveKitInstrumentComponent(srcInstrumentUuid, srcVoiceIdx, srcComponentIdx,
                                      dstInstrumentUuid, dstSlotIdx);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::removeComponentFromKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .removeComponentFromKitInstrumentVoice(instrumentUuid, voiceIdx, componentIdx);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx, componentIdx]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .removeComponentFromKitInstrumentVoice(instrumentUuid, voiceIdx, componentIdx);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::removeVoiceFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .removeVoiceFromKitInstrument(instrumentUuid, voiceIdx);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .removeVoiceFromKitInstrument(instrumentUuid, voiceIdx);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).setVoiceNameInKitInstrument(
       instrumentUuid, voiceIdx, name);
   static constexpr size_t MaxStringSize = 64;
   m_deferToRt.callAsync([this, &instrumentUuid, voiceIdx, fsName = util::FixedSizeString<MaxStringSize>(name)]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments).setVoiceNameInKitInstrument(instrumentUuid, voiceIdx, fsName);
   });
   emitDataChanged(m_loaderData, true);
}

void Instruments::incKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .incKitInstrumentRefCount(uuid);
}
void Instruments::decKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .decKitInstrumentRefCount(uuid);
}
void Instruments::incMelodicInstrumentRefCount(
    const util::Identifiable::UUID& uuid)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .incMelodicInstrumentRefCount(uuid);
}
void Instruments::decMelodicInstrumentRefCount(
    const util::Identifiable::UUID& uuid)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .decMelodicInstrumentRefCount(uuid);
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

std::string Instruments::serializeKitInstruments() const
{
   return nlohmann::json(m_loaderData.kitInstruments)
       .dump()
       .c_str();
}

std::string Instruments::serializeMelodicInstruments() const
{
   return nlohmann::json(m_loaderData.melodicInstruments)
       .dump()
       .c_str();
}

void Instruments::fillReferencesToMD(musicDevice::MusicDevice* pMusicDevice)
{
   //TODO
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).fillReferences(pMusicDevice);
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments).fillReferences(pMusicDevice);
   emitDataChanged(m_loaderData, false);
}

void Instruments::removeReferencesToMD(musicDevice::MusicDevice* pMusicDevice)
{
   //TODO
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).removeReferences(pMusicDevice);
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments).removeReferences(pMusicDevice);
   emitDataChanged(m_loaderData, false);
}

void Instruments::reEmitSignals()
{
   emitDataChanged(m_loaderData, false);
}

void Instruments::saveIfDirty()
{
   if(m_parameterCacheDirty)
   {
      m_persister.save(m_loaderData);
      m_parameterCacheDirty = false;
   }
}

void Instruments::setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                        int componentIdx, float amp)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .setKitComponentAmp(uuid, voiceIdx, componentIdx, amp);
   m_deferToRt.callAsync([this, uuid, voiceIdx, componentIdx, amp]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .setKitComponentAmp(uuid, voiceIdx, componentIdx, amp);
   });
   emitKitComponentAmpChanged(uuid, voiceIdx, componentIdx, amp);
}

void Instruments::setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .setKitVoiceAmp(uuid, voiceIdx, amp);
   m_deferToRt.callAsync([this, uuid, voiceIdx, amp]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments)
          .setKitVoiceAmp(uuid, voiceIdx, amp);
   });
   emitKitVoiceAmpChanged(uuid, voiceIdx, amp); 
}

void Instruments::setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                               int componentIdx, float amp)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .setMelodicComponentAmp(uuid, componentIdx, amp);
   emitMelodicComponentAmpChanged(uuid, componentIdx, amp);
}

void Instruments::invokeQueueActions()
{
   m_deferToRt.process();
}

bool Instruments::hasKitInstrument(
    util::Identifiable::UUIDView uuid) const
{
   const auto it = std::ranges::find_if(
       m_loaderData.kitInstruments,
       [&uuid](const loader::KitInstrument& instr) { return uuid == instr.idView(); });
   return it != m_loaderData.kitInstruments.end();
}

bool Instruments::hasMelodicInstrument(
    util::Identifiable::UUIDView uuid) const
{
   const auto it =
       std::ranges::find_if(m_loaderData.melodicInstruments,
                            [&uuid](const loader::MelodicInstrument& instr) {
                               return uuid == instr.idView();
                            });
   return it != m_loaderData.melodicInstruments.end();
}

void Instruments::updateParameterUI()
{
   for(const auto& instr : m_loaderData.kitInstruments) 
   { 
      //TODO instr.updateParameterUI();
   }
   for(const auto& instr : m_loaderData.melodicInstruments) 
   { 
      // TODO instr.updateParameterUI();
   }
}
