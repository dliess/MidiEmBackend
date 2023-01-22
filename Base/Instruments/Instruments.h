#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <functional>
#include <vector>

#include "InstrumentsData.h"
#include "Identifiable.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "Meta.h"
#include "Settings_old.h"
#include "function_ref.h"
#include "DoubleBuffer.h"

// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
// clang-format on

namespace base::instruments
{

struct Instruments   //: public utils::Settings<Instruments>
{
   Instruments(
       musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept;
   /*
   // ============== Settings ===============
   using Settings = Data;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
    */
   using Cb = std::function<void(void)>;
   void registerForDataChange(Cb cb) noexcept;
   void triggerChanged() noexcept;

   void createKitInstrument(std::string name) noexcept;
   void removeKitInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            std::string name) noexcept;

   void createMelodicInstrument(std::string name) noexcept;
   void removeMelodicInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                std::string name) noexcept;

   void createNewSlotInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept;

   void addVoiceToMelodicInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept;
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
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept;
   void addVoiceToKitInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept;
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

    std::string serializeKitInstruments() const;
    std::string serializeMelodicInstruments() const;

   [[nodiscard]] const Instrument* getInstrumentByUuid(
       util::Identifiable::UUIDView) noexcept;

   inline void withInstrumentRt(util::Identifiable::UUIDView uuid,
                                 util::function_ref<void(const Instrument&)> cb);
   inline void withKitInstrumentRt(util::Identifiable::UUIDView uuid,
                                 util::function_ref<void(const KitInstrument&)> cb);
   inline void withMelodicInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const MelodicInstrument&)> cb);

private:
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   util::DoubleBuffer<Data> m_doubleBufferedData;
   std::vector<Cb> m_subscribers;
};

}   // namespace base::instruments

#include "Instruments.inl"

#endif