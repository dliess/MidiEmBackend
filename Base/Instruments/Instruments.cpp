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
#include "MelodicInstrumentCopyer.h"
#include "MusicDeviceContainer.h"
#include "KitInstrumentModifier.h"
#include "MelodicInstrumentModifier.h"

using namespace base;
using namespace base::instruments;

Instruments::Instruments(
    musicDevice::factory::DataHolder& rFactoryDataHolder,
    musicDevice::MusicDeviceContainer& rMDContainer) noexcept :
    m_rFactoryDataHolder(rFactoryDataHolder),
    m_rMDContainer(rMDContainer),
    m_persister(
        std::make_unique<util::FilePersister>("Instruments", "settings.json"),
        rFactoryDataHolder)
{
   try
   {
      auto data = m_persister.load();
      for(auto& kitInstrument : data.kitInstruments)
      {
         loader::KitInstrumentModifier(kitInstrument).fillDescrReferences(m_rFactoryDataHolder);
         insertKitInstrument(kitInstrument, DoEmitChanged::No);
      }
      for(auto& melodicInstrument : data.melodicInstruments)
      {
         loader::MelodicInstrumentModifier(melodicInstrument).fillDescrReferences(m_rFactoryDataHolder);
         insertMelodicInstrument(melodicInstrument, DoEmitChanged::No);
      }
      emitDataChanged(m_loaderData);
   }
   catch (std::exception& e)
   {
      spdlog::error(
          "Error loading Instruments settings, its maybe the first run: {}",
          e.what());
   }
   onDataChanged([this](const auto& data) { m_dirty = true; });
   onKitVoiceAmpChanged([this](const auto& uuid, int voiceIdx, float amp) {
      m_dirty = true;
   });
   onKitComponentAmpChanged([this](const auto& uuid, int voiceIdx, int componentIdx, float amp) {
      m_dirty = true;
   });
   onMelodicComponentAmpChanged([this](const auto& uuid, int componentIdx, float amp) {
      m_dirty = true;
   });
   onKitVoiceNoteOffsetChanged([this](const auto& uuid, int voiceIdx, int noteOffset) {
      m_dirty = true;
   });
   onKitComponentNoteOffsetChanged([this](const auto& uuid, int voiceIdx, int componentIdx, int noteOffset) {
      m_dirty = true;
   });
   onMelodicComponentNoteOffsetChanged([this](const auto& uuid, int componentIdx, int noteOffset) {
      m_dirty = true;
   });
}

void Instruments::createKitInstrument(std::string name)
{
   auto uuid = loader::KitInstrumentsModifier(m_loaderData.kitInstruments).createKitInstrument(
       std::move(name));
   m_deferToRt.callAsync([this, uuid]() { m_rtData.kitInstruments.emplace_back(uuid); });
   emitDataChanged(m_loaderData);
}

void Instruments::insertKitInstrument(loader::KitInstrument& kitInstrument, DoEmitChanged doEmitChanged)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .insertKitInstrument(kitInstrument);
   m_deferToRt.callAsync([this, copy = std::make_unique<loader::KitInstrument>(kitInstrument)]() mutable { 
      m_rtData.kitInstruments.push_back(KitInstrumentCopyer::copy(m_rMDContainer, *copy));
      m_deferToLoader.callAsync([c = std::move(copy)]() mutable { c.reset(); });
   });
   if(doEmitChanged == DoEmitChanged::Yes)
   {
      emitDataChanged(m_loaderData);
   }
}

bool Instruments::hasSameInstrument(const loader::KitInstrument& kitInstrument) const
{
   for (const auto& e : m_loaderData.kitInstruments)
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
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).removeKitInstrument(instrumentId);
   m_deferToRt.callAsync([this, instrumentId]() {
      rt::KitInstrumentsModifier(m_rtData.kitInstruments).removeKitInstrument(instrumentId);
   });
   emitDataChanged(m_loaderData);
}

void Instruments::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments)
       .renameKitInstrument(instrumentId, name);
   emitDataChanged(m_loaderData);
}

void Instruments::createMelodicInstrument(std::string name)
{
   auto uuid = loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .createMelodicInstrument(std::move(name));
   m_deferToRt.callAsync([this, uuid]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments).createMelodicInstrument(uuid);
   });
   emitDataChanged(m_loaderData);
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

void Instruments::insertMelodicInstrument(loader::MelodicInstrument& melodicInstrument, DoEmitChanged doEmitChanged)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .insertMelodicInstrument(melodicInstrument);
   m_deferToRt.callAsync([this, copy = std::make_unique<loader::MelodicInstrument>(melodicInstrument)]() mutable {
      m_rtData.melodicInstruments.push_back(MelodicInstrumentCopyer::copy(m_rMDContainer, *copy));
      m_deferToLoader.callAsync([c = std::move(copy)]() mutable { c.reset(); });
   });
   if(doEmitChanged == DoEmitChanged::Yes)
   {
      emitDataChanged(m_loaderData);
   }
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeMelodicInstrument(instrumentId);
   m_deferToRt.callAsync([this, instrumentId]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
          .removeMelodicInstrument(instrumentId);
   });
   emitDataChanged(m_loaderData);
}

void Instruments::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, const std::string& name)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .renameMelodicInstrument(instrumentId, name);
   emitDataChanged(m_loaderData);
}

Void Instruments::createNewVoiceInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   return loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .createNewVoiceInMelodicInstrument(m_rFactoryDataHolder, instrumentUuid, sdUuid, sdVoiceIdx).map(
      [&,this](int componentIdx){
         m_deferToRt.callAsync([this, componentIdx, instrumentUuid, sdUuid, sdVoiceIdx]() {
            rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
                .createNewVoiceInMelodicInstrument(m_rMDContainer, componentIdx, instrumentUuid, sdUuid, sdVoiceIdx);
      });
      emitDataChanged(m_loaderData);
   });
}

Void Instruments::addComponentToMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx)
{
   return loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .addComponentToMelodicInstrumentVoice(m_rFactoryDataHolder, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx).map(
      [&,this](int componentIdx){
         m_deferToRt.callAsync([this, componentIdx, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx]() {
            rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
                .addComponentToMelodicInstrumentVoice(m_rMDContainer, componentIdx, instrumentUuid, voiceIdx, sdUuid, sdVoiceIdx);
      });
      emitDataChanged(m_loaderData);
   });
}

void Instruments::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeComponentFromMelodicInstrumentVoice(instrumentUuid, voiceIdx, componentIdx);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx, componentIdx]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
          .removeComponentFromMelodicInstrumentVoice(instrumentUuid, voiceIdx, componentIdx);
   });
   emitDataChanged(m_loaderData);
}

void Instruments::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .removeVoiceFromMelodicInstrument(instrumentUuid, voiceIdx);
   m_deferToRt.callAsync([this, instrumentUuid, voiceIdx]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
          .removeVoiceFromMelodicInstrument(instrumentUuid, voiceIdx);
   });
   emitDataChanged(m_loaderData);
}

void Instruments::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid,
    int componentIdx, int noteOffset)
{
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments)
       .setNoteOffsetInMelodicInstrumentComponent(instrumentUuid, componentIdx, noteOffset);
   m_deferToRt.callAsync([this, instrumentUuid, componentIdx, noteOffset]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
          .setNoteOffsetInMelodicInstrumentComponent(instrumentUuid, componentIdx, noteOffset);
   });
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
   emitDataChanged(m_loaderData);
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
   emitDataChanged(m_loaderData);
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
   emitDataChanged(m_loaderData);
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
   emitDataChanged(m_loaderData);
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
   emitDataChanged(m_loaderData);
}

void Instruments::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name)
{
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).setVoiceNameInKitInstrument(
       instrumentUuid, voiceIdx, name);
   emitDataChanged(m_loaderData);
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

void Instruments::removeReferencesToMD(musicDevice::MusicDevice* pMusicDevice)
{
   //TODO
   loader::KitInstrumentsModifier(m_loaderData.kitInstruments).removeReferences(pMusicDevice);
   loader::MelodicInstrumentsModifier(m_loaderData.melodicInstruments).removeReferences(pMusicDevice);
   emitDataChanged(m_loaderData);
}

void Instruments::reEmitSignals()
{
   emitDataChanged(m_loaderData);
}

void Instruments::saveIfDirty()
{
   if(m_dirty)
   {
      m_persister.save(m_loaderData);
      m_dirty = false;
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
   m_deferToRt.callAsync([this, uuid, componentIdx, amp]() {
      rt::MelodicInstrumentsModifier(m_rtData.melodicInstruments)
          .setMelodicComponentAmp(uuid, componentIdx, amp);
   });
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

Ret<InstrumentRtRef> Instruments::getInstrumentRtRef(util::Identifiable::UUIDView uuid)
{
   auto it1 = elementWithUuid(m_rtData.kitInstruments, uuid);
   if(it1 != m_rtData.kitInstruments.end())
   {
      return InstrumentRtRef(*it1);
   }
   auto it2 = elementWithUuid(m_rtData.melodicInstruments, uuid);
   if(it2 != m_rtData.melodicInstruments.end())
   {
      return InstrumentRtRef(*it2);
   }
   return tl::unexpected(Error::uuidNotFound);
}

Ret<KitInstrumentRtRef> Instruments::getKitInstrumentRtRef(util::Identifiable::UUIDView uuid)
{
   auto it1 = elementWithUuid(m_rtData.kitInstruments, uuid);
   if(it1 != m_rtData.kitInstruments.end())
   {
      return KitInstrumentRtRef(*it1);
   }
   return tl::unexpected(Error::uuidNotFound);
}

Ret<MelodicInstrumentRtRef> Instruments::getMelodicInstrumentRtRef(util::Identifiable::UUIDView uuid)
{
   auto it2 = elementWithUuid(m_rtData.melodicInstruments, uuid);
   if(it2 != m_rtData.melodicInstruments.end())
   {
      return MelodicInstrumentRtRef(*it2);
   }
   return tl::unexpected(Error::uuidNotFound);
}

Ret<const base::musicDevice::description::sound::Parameter*> 
Instruments::getParameterDescriptionOfKit(util::Identifiable::UUIDView uuid, int voiceIdx, int componentIdx, int ParameterIdx)
{
   auto it = elementWithUuid(m_loaderData.kitInstruments, uuid);
   if(it != m_loaderData.kitInstruments.end())
   {
      return it->parameterDescription(voiceIdx, componentIdx, ParameterIdx);
   }
   return tl::unexpected(Error::uuidNotFound);
}

Ret<const base::musicDevice::description::sound::Parameter*> 
Instruments::getParameterDescriptionOfMelodic(util::Identifiable::UUIDView uuid, int componentIdx, int parameterIdx)
{
   auto it = elementWithUuid(m_loaderData.melodicInstruments, uuid);
   if(it != m_loaderData.melodicInstruments.end())
   {
      return it->parameterDescription(componentIdx, parameterIdx);
   }
   return tl::unexpected(Error::uuidNotFound);
}
