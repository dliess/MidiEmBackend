#include "Instruments.h"

#include <spdlog/spdlog.h>

#include "InstrumentsModifier.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "SoundSection.h"

using namespace base::instruments;

Instruments::Instruments(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
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
   data.withNonRtLocked([this, &name](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .createKitInstrument(std::move(name));
   });
   triggerChanged();
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   data.withNonRtLocked([this, &instrumentId](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .removeKitInstrument(instrumentId);
   });
   triggerChanged();
}

void Instruments::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   data.withNonRtLocked([this, &instrumentId, &name](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .renameKitInstrument(instrumentId, std::move(name));
   });
   triggerChanged();
}

void Instruments::createMelodicInstrument(std::string name) noexcept
{
   data.withNonRtLocked([this, &name](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .createMelodicInstrument(std::move(name));
   });
   triggerChanged();
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   data.withNonRtLocked([this, &instrumentId](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .removeMelodicInstrument(instrumentId);
   });
   triggerChanged();
}

void Instruments::renameMelodicInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   data.withNonRtLocked([this, &instrumentId, &name](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .renameMelodicInstrument(instrumentId, std::move(name));
   });
   triggerChanged();
}

void Instruments::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   data.withNonRtLocked(
       [this, &instrumentUuid, &soundDeviceUuid, voiceIdx](auto& data) {
          InstrumentsModifier(data, m_rFactoryDataHolder)
              .createNewSlotInMelodicInstrument(instrumentUuid, soundDeviceUuid,
                                                voiceIdx);
       });
   triggerChanged();
}

void Instruments::addVoiceToMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   data.withNonRtLocked([this, &instrumentUuid, slotIdx, &soundDeviceUuid,
                         voiceIdx](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .addVoiceToMelodicInstrumentSlot(instrumentUuid, slotIdx,
                                           soundDeviceUuid, voiceIdx);
   });
   triggerChanged();
}

void Instruments::removeVoiceFromMelodicInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx) noexcept
{
   data.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, compositeIdx](auto& data) {
          InstrumentsModifier(data, m_rFactoryDataHolder)
              .removeVoiceFromMelodicInstrumentSlot(instrumentUuid, slotIdx,
                                                    compositeIdx);
       });
   triggerChanged();
}

void Instruments::removeSlotFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   data.withNonRtLocked([this, &instrumentUuid, slotIdx](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .removeSlotFromMelodicInstrument(instrumentUuid, slotIdx);
   });
   triggerChanged();
}

void Instruments::setNoteOffsetInMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   data.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, compositeIdx, noteOffset](auto& data) {
          InstrumentsModifier(data, m_rFactoryDataHolder)
              .setNoteOffsetInMelodicInstrumentVoice(instrumentUuid, slotIdx,
                                                     compositeIdx, noteOffset);
       });
   triggerChanged();
}

void Instruments::setCompositeNameInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string name) noexcept
{
   data.withNonRtLocked([this, &instrumentUuid, slotIdx, &name](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .setCompositeNameInMelodicInstrument(instrumentUuid, slotIdx,
                                               std::move(name));
   });
   triggerChanged();
}

void Instruments::createNewSlotInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   data.withNonRtLocked(
       [this, &instrumentUuid, soundDeviceUuid, voiceIdx](auto& data) {
          InstrumentsModifier(data, m_rFactoryDataHolder)
              .createNewSlotInKitInstrument(instrumentUuid, soundDeviceUuid,
                                            voiceIdx);
       });
   triggerChanged();
}

void Instruments::addVoiceToKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   data.withNonRtLocked(
       [this, &instrumentUuid, slotIdx, soundDeviceUuid, voiceIdx](auto& data) {
          InstrumentsModifier(data, m_rFactoryDataHolder)
              .addVoiceToKitInstrumentSlot(instrumentUuid, slotIdx,
                                           soundDeviceUuid, voiceIdx);
       });
   triggerChanged();
}

void Instruments::moveKitInstrumentSlotVoice(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
    int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstSlotIdx) noexcept
{
   data.withNonRtLocked([this, &](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .moveKitInstrumentSlotVoice(srcInstrumentUuid, srcSlotIdx,
                                      srcCompositeIdx, dstInstrumentUuid,
                                      dstSlotIdx);
   });
   triggerChanged();
}

void Instruments::removeVoiceFromKitInstrumentSlot(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx) noexcept
{
   data.withNonRtLocked([this, &](auto& data) {
      InstrumentsModifier(data, m_rFactoryDataHolder)
          .removeVoiceFromKitInstrumentSlot(instrumentUuid, slotIdx,
                                            compositeIdx);
      triggerChanged();
}

void Instruments::removeSlotFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept
{
   data.withNonRtLocked([this, &](auto& data) {
         InstrumentsModifier(data, m_rFactoryDataHolder)
             .removeSlotFromKitInstrument(instrumentUuid, slotIdx);
         triggerChanged();
}

void Instruments::setNoteOffsetInKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    int compositeIdx, int noteOffset) noexcept
{
   data.withNonRtLocked([this, &](auto& data) {
            InstrumentsModifier(data, m_rFactoryDataHolder)
                .setNoteOffsetInKitInstrumentVoice(instrumentUuid, slotIdx,
                                                   compositeIdx, noteOffset);
            triggerChanged();
}

void Instruments::setCompositeNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int slotIdx,
    const std::string& name) noexcept
{
   data.withNonRtLocked([this, &](auto& data) {
               InstrumentsModifier(data, m_rFactoryDataHolder)
                   .setCompositeNameInKitInstrument(instrumentUuid, slotIdx,
                                                    std::move(name));
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

Instrument* Instruments::getInstrumentByUuid(
    util::Identifiable::UUIDView uuid) noexcept
{
               auto it1 = elementWithUuid(data.kitInstruments, uuid);
               if (it1 != data.kitInstruments.end())
               {
                  return &(*it1);
               }
               auto it2 = elementWithUuid(data.melodicInstruments, uuid);
               if (it2 != data.melodicInstruments.end())
               {
                  return &(*it2);
               }
               return nullptr;
}