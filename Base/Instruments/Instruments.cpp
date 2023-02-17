#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "InstrumentsMDChangeHandler.h"
#include "InstrumentsModifier.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "SoundSection.h"

using namespace base::instruments;

Instruments::Instruments(
    musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
   rFactoryDataHolder.onMusicDeviceAdded([this](auto md) {
      InstrumentsMDChangeHandler(m_doubleBufferedData).add(md);
      triggerChanged();
   });
   rFactoryDataHolder.onMusicDeviceAboutToRemove([this](auto md) {
      InstrumentsMDChangeHandler(m_doubleBufferedData).remove(md);
      triggerChanged();
   });
}

void Instruments::registerForDataChange(Cb cb) { m_subscribers.push_back(cb); }

void Instruments::triggerChanged()
{
   for (auto& cb : m_subscribers) cb();
}

void Instruments::createKitInstrument(std::string name)
{
   KitInstrument kitInstrument(name);
   m_doubleBufferedData.withNonRtLocked(
       [this, &kitInstrument](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .insertKitInstrument(kitInstrument);
       });
   triggerChanged();
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentId](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeKitInstrument(instrumentId);
   });
   triggerChanged();
}

void Instruments::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentId, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .renameKitInstrument(instrumentId, std::move(name));
       });
   triggerChanged();
}

void Instruments::createMelodicInstrument(std::string name)
{
   MelodicInstrument melodicInstrument(
       name, std::make_shared<MelodicInstrument::RtData>());
   m_doubleBufferedData.withNonRtLocked(
       [this, &melodicInstrument](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .insertMelodicInstrument(melodicInstrument);
       });
   triggerChanged();
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId)
{
   m_doubleBufferedData.withNonRtLocked([this, &instrumentId](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeMelodicInstrument(instrumentId);
   });
   triggerChanged();
}

void Instruments::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentId, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .renameMelodicInstrument(instrumentId, std::move(name));
       });
   triggerChanged();
}

void Instruments::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = InstrumentsModifier::createParameterCache(
       m_rFactoryDataHolder, soundDeviceUuid, voiceIdx);
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid,
                                         &soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewSlotInMelodicInstrument(instrumentUuid, soundDeviceUuid,
                                            voiceIdx, paramCache);
   });
   triggerChanged();
}

void Instruments::addVoiceToMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = InstrumentsModifier::createParameterCache(
       m_rFactoryDataHolder, soundDeviceUuid, voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         &soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addVoiceToMelodicInstrumentSlot(
              instrumentUuid, slotIdx, soundDeviceUuid, voiceIdx, paramCache);
   });
   triggerChanged();
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
   triggerChanged();
}

void Instruments::removeSlotFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .removeSlotFromMelodicInstrument(instrumentUuid, slotIdx);
       });
   triggerChanged();
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
   triggerChanged();
}

void Instruments::setCompositeNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string name)
{
   m_doubleBufferedData.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, &name](auto& nonRtData) {
          InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
              .setCompositeNameInMelodicInstrument(instrumentUuid, slotIdx,
                                                   std::move(name));
       });
   triggerChanged();
}

void Instruments::createNewSlotInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = InstrumentsModifier::createParameterCache(
       m_rFactoryDataHolder, soundDeviceUuid, voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, soundDeviceUuid,
                                         voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .createNewSlotInKitInstrument(instrumentUuid, soundDeviceUuid,
                                        voiceIdx, paramCache);
   });
   triggerChanged();
}

void Instruments::addVoiceToKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto paramCache = InstrumentsModifier::createParameterCache(
       m_rFactoryDataHolder, soundDeviceUuid, voiceIdx);
   if (!paramCache)
   {
      spdlog::error("Could not create parameter cache");
      return;
   }
   m_doubleBufferedData.withNonRtLocked([this, &instrumentUuid, slotIdx,
                                         soundDeviceUuid, voiceIdx,
                                         &paramCache](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .addVoiceToKitInstrumentSlot(instrumentUuid, slotIdx, soundDeviceUuid,
                                       voiceIdx, paramCache);
   });
   triggerChanged();
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
   triggerChanged();
}

void Instruments::removeVoiceFromKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeVoiceFromKitInstrumentSlot(instrumentUuid, slotIdx,
                                            compositeIdx);
   });
   triggerChanged();
}

void Instruments::removeSlotFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .removeSlotFromKitInstrument(instrumentUuid, slotIdx);
   });
   triggerChanged();
}

void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setNoteOffsetInKitInstrumentVoice(instrumentUuid, slotIdx,
                                             compositeIdx, noteOffset);
   });
   triggerChanged();
}

void Instruments::setCompositeNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name)
{
   m_doubleBufferedData.withNonRtLocked([&, this](auto& nonRtData) {
      InstrumentsModifier(nonRtData, m_rFactoryDataHolder)
          .setCompositeNameInKitInstrument(instrumentUuid, slotIdx,
                                           std::move(name));
   });
   triggerChanged();
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
   return meta::serialize(m_doubleBufferedData.nonRt().kitInstruments)
       .dump()
       .c_str();
}

std::string Instruments::serializeMelodicInstruments() const
{
   return meta::serialize(m_doubleBufferedData.nonRt().melodicInstruments)
       .dump()
       .c_str();
}
