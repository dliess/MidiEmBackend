#ifndef INSTRUMENTS_MODIFIER_H
#define INSTRUMENTS_MODIFIER_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments
{
struct InstrumentsModifier
{
   InstrumentsModifier(
       Data& rData,
       base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept;

   void insertKitInstrument(KitInstrument kitInstrument) noexcept;
   void removeKitInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            std::string name) noexcept;

   void insertMelodicInstrument(MelodicInstrument melodicInstrument) noexcept;
   void removeMelodicInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                std::string name) noexcept;

   void createNewSlotInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
       std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept;

   void addVoiceToMelodicInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
       std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept;
   void removeVoiceFromMelodicInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx) noexcept;
   void removeSlotFromMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept;
   void setNoteOffsetInMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx, int noteOffset) noexcept;
   void setCompositeNameInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const std::string name) noexcept;

   void createNewSlotInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
       std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept;
   void addVoiceToKitInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx,
       std::shared_ptr<Voice::ParameterCache> parameterCache) noexcept;
   void moveKitInstrumentSlotVoice(
       const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
       int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
       int dstSlotIdx) noexcept;
   void removeVoiceFromKitInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx) noexcept;
   void removeSlotFromKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx) noexcept;
   void setNoteOffsetInKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx, int noteOffset) noexcept;
   void setCompositeNameInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const std::string& name) noexcept;

   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);

private:
   Data& m_rData;
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   static constexpr int MAX_VOICES_IN_SLOT = 4;
};

}   // namespace base::instruments
#endif