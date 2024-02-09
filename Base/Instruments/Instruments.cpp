#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "FilePersister.h"
#include "InstrumentComponentParameterCacheCreator.h"
#include "InstrumentsMDRefSetter.h"
#include "InstrumentsModifier.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "SoundSection.h"
#include "FixedSizeString.h"

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

void Instruments::insertKitInstrument(KitInstrument& kitInstrument)
{
   spdlog::info("Inserting KitInstrument with uuid: {}", util::uuid2Str(kitInstrument.id()));
   KitInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
       .initParameterCaches(kitInstrument, *this);
   m_doubleBufferedData.withNonRtLocked(
       [this, &kitInstrument](auto& nonRtData) {
          nonRtData.kitInstruments.push_back(kitInstrument);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

bool Instruments::hasSameInstrument(const KitInstrument& kitInstrument) const
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

bool Instruments::hasSameInstrument(
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

void Instruments::insertMelodicInstrument(MelodicInstrument& melodicInstrument)
{
   spdlog::info("Inserting MelodicInstrument with uuid: {}", util::uuid2Str(melodicInstrument.id()));
   MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
       .initParameterCaches(melodicInstrument, *this);
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

void Instruments::createNewVoiceInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   auto instrIt = std::ranges::find_if(
       m_doubleBufferedData.nonRt().melodicInstruments,
       [&instrumentUuid](const auto& e) { return e.id() == instrumentUuid; });
   if (instrIt == m_doubleBufferedData.nonRt().melodicInstruments.end())
   {
      return;
   }
   auto paramCache =
       MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
           .createParameterCacheForNextMatchingComponentInNextVoice(
               *instrIt, sdUuid, sdVoiceIdx, *this);
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, &sdUuid,
                                         sdVoiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewVoiceInMelodicInstrument(instrumentUuid, sdUuid, sdVoiceIdx,
                                             paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::addComponentToMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   auto instrIt = std::ranges::find_if(
       m_doubleBufferedData.nonRt().melodicInstruments,
       [&instrumentUuid](const auto& e) { return e.id() == instrumentUuid; });
   if (instrIt == m_doubleBufferedData.nonRt().melodicInstruments.end())
   {
      return;
   }
   auto paramCache =
       MelodicInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
           .createParameterCacheForNextMatchingComponentInVoice(
               *instrIt, voiceIdx, sdUuid, sdVoiceIdx, *this);
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, voiceIdx,
                                         &sdUuid, sdVoiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addComponentToMelodicInstrumentVoice(instrumentUuid, voiceIdx,
                                                sdUuid, sdVoiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, voiceIdx, componentIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeComponentFromMelodicInstrumentVoice(
                  instrumentUuid, voiceIdx, componentIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, voiceIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeVoiceFromMelodicInstrument(instrumentUuid, voiceIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid,
    int componentIdx, int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, 
                                         componentIdx,
                                         noteOffset](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInMelodicInstrumentComponent(instrumentUuid, 
                                                     componentIdx, noteOffset);
   });
   emitMelodicComponentNoteOffsetChanged(instrumentUuid, componentIdx, noteOffset); // TODO: emit from here or connect to signal?
}

void Instruments::setNoteOffsetInKitInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, voiceIdx,
                                         componentIdx,
                                         noteOffset](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInKitInstrumentComponent(instrumentUuid, voiceIdx,
                                                 componentIdx, noteOffset);
   });
   emitKitComponentNoteOffsetChanged(instrumentUuid, voiceIdx, componentIdx,
                                     noteOffset); // TODO: emit from here or connect to signal? 
}
void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, voiceIdx,
                                         noteOffset](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInKitInstrumentVoice(instrumentUuid, voiceIdx, noteOffset);
   });
   emitKitVoiceNoteOffsetChanged(instrumentUuid, voiceIdx, noteOffset); // TODO: emit from here or connect to signal?
}

void Instruments::setVoiceNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, voiceIdx, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .setVoiceNameInMelodicInstrument(instrumentUuid, voiceIdx, name);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::createNewVoiceInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   auto instrIt = std::ranges::find_if(
       m_doubleBufferedData.nonRt().kitInstruments,
       [&instrumentUuid](const auto& e) { return e.id() == instrumentUuid; });
   if (instrIt == m_doubleBufferedData.nonRt().kitInstruments.end())
   {
      return;
   }
   auto paramCache = KitInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
                         .createParameterCacheForNewComponentInNextVoice(
                             *instrIt, sdUuid, sdVoiceIdx, *this);
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, sdUuid,
                                         sdVoiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewVoiceInKitInstrument(instrumentUuid, sdUuid, sdVoiceIdx,
                                         paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::addComponentToKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   auto instrIt = std::ranges::find_if(
       m_doubleBufferedData.nonRt().kitInstruments,
       [&instrumentUuid](const auto& e) { return e.id() == instrumentUuid; });
   if (instrIt == m_doubleBufferedData.nonRt().kitInstruments.end())
   {
      return;
   }
   auto paramCache = KitInstrumentsParameterCacheCreator(m_rFactoryDataHolder)
                         .createParameterCacheForNewComponentInVoice(
                             *instrIt, voiceIdx, sdUuid, sdVoiceIdx, *this);
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, voiceIdx,
                                         sdUuid, sdVoiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addComponentToKitInstrumentVoice(instrumentUuid, voiceIdx, sdUuid,
                                            sdVoiceIdx, paramCache);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::moveKitInstrumentComponent(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
    int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .moveKitInstrumentComponent(srcInstrumentUuid, srcVoiceIdx,
                                      srcComponentIdx, dstInstrumentUuid,
                                      dstSlotIdx);
   });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeComponentFromKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, voiceIdx, componentIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeComponentFromKitInstrumentVoice(instrumentUuid, voiceIdx,
                                                     componentIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::removeVoiceFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, voiceIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeVoiceFromKitInstrument(instrumentUuid, voiceIdx);
       });
   emitDataChanged(m_doubleBufferedData.nonRt(), true);
}

void Instruments::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name)
{
   InstrumentsModifier(m_loaderData, m_rFactoryDataHolder)
              .setVoiceNameInKitInstrument(instrumentUuid, voiceIdx, name);
   static constexpr size_t MaxStringSize = 64;
   m_asyncCaller.callAsync([this, &instrumentUuid, voiceIdx, fsName = util::FixedSizeString<MaxStringSize>(name)]() {
      // InstrumentsModifier(m_rtData, m_rFactoryDataHolder)
      //            .setVoiceNameInKitInstrument(instrumentUuid, voiceIdx, fsName);
   });

   emitDataChanged(m_loaderData, true);
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

void Instruments::kitParamChanged(
    const util::Identifiable::UUID& uuid, int voiceIdx, int componentIdx,
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   auto& instr = util::getByUuid(m_doubleBufferedData.nonRt().kitInstruments, uuid);
   auto parameterCache = instr.voices().at(voiceIdx).components.at(componentIdx).parameterCache();
   parameterCache->setParameterBackup(parameterIdx, parameterAttr, value);
   m_parameterCacheDirty = true;
}

void Instruments::melodicParamChanged(
    const util::Identifiable::UUID& uuid, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value)
{
   auto& instr = util::getByUuid(m_doubleBufferedData.nonRt().melodicInstruments, uuid);
   auto component = instr.getFirstComponent(componentIdx);
   if(component)
   {
      component->parameterCache()->setParameterBackup(parameterIdx, parameterAttr, value);
   }
   m_parameterCacheDirty = true;
}

void Instruments::saveIfDirty()
{
   if(m_parameterCacheDirty)
   {
      m_persister.save(m_doubleBufferedData.nonRt());
      m_parameterCacheDirty = false;
   }
}

void Instruments::setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                        int componentIdx, float amp)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid, voiceIdx, componentIdx, amp](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setKitComponentAmp(uuid, voiceIdx, componentIdx, amp);
   });
   emitKitComponentAmpChanged(uuid, voiceIdx, componentIdx, amp); // TODO emit from here or connect to signal?
}

void Instruments::setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid, voiceIdx, amp](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setKitVoiceAmp(uuid, voiceIdx, amp);
   });
   emitKitVoiceAmpChanged(uuid, voiceIdx, amp); // TODO emit from here or connect to signal?
}

void Instruments::setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                               int componentIdx, float amp)
{
   m_doubleBufferedData.withNonRtLocked([this, &uuid, componentIdx, amp](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setMelodicComponentAmp(uuid, componentIdx, amp);
   });
   // emitMelodicComponentAmpChanged(uuid, componentIdx, amp); // TODO emit from here or connect to signal?
}

void Instruments::invokeQueueActions()
{
   m_asyncCaller.process();
}
