#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <functional>
#include <vector>

#include "DoubleBuffer.h"
#include "Identifiable.h"
#include "InstrumentsData.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "Meta.h"
#include "Settings_old.h"
#include "function_ref.h"

// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
// clang-format on

namespace base::instruments
{
struct Instruments   //: public utils::Settings<Instruments>
{
   Instruments(musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept;
   /*
   // ============== Settings ===============
   using Settings = Data;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
    */
   using Cb = std::function<void(void)>;
   void registerForDataChange(Cb cb);
   void triggerChanged();

   void createKitInstrument(std::string name);
   void insertKitInstrument(const KitInstrument& kitInstrument);
   void removeKitInstrument(const util::Identifiable::UUID& instrumentId);
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            std::string name);

   void createMelodicInstrument(std::string name);
   void insertMelodicInstrument(const MelodicInstrument& melodicInstrument);
   void removeMelodicInstrument(const util::Identifiable::UUID& instrumentId);
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                std::string name);

   void createNewSlotInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx);

   void addVoiceToMelodicInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx);
   void removeVoiceFromMelodicInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx);
   void removeSlotFromMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx);
   void setNoteOffsetInMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx, int noteOffset);
   void setCompositeNameInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const std::string name);

   void createNewSlotInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx);
   void addVoiceToKitInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx);
   void moveKitInstrumentSlotVoice(
       const util::Identifiable::UUID& srcInstrumentUuid, int srcSlotIdx,
       int srcCompositeIdx, const util::Identifiable::UUID& dstInstrumentUuid,
       int dstSlotIdx);
   void removeVoiceFromKitInstrumentSlot(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx);
   void removeSlotFromKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx);
   void setNoteOffsetInKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       int compositeIdx, int noteOffset);
   void setCompositeNameInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int slotIdx,
       const std::string& name);

   std::string serializeKitInstruments() const;
   std::string serializeMelodicInstruments() const;

   [[nodiscard]] const Instrument* getInstrumentByUuid(
       util::Identifiable::UUIDView);

   inline void withInstrumentRt(util::Identifiable::UUIDView uuid,
                                util::function_ref<void(const Instrument&)> cb);
   inline void withKitInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const KitInstrument&)> cb);
   inline void withMelodicInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const MelodicInstrument&)> cb);


private:
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   util::DoubleBuffer<Data> m_doubleBufferedData;
   std::vector<Cb> m_subscribers;
   friend class InstrumentsMDChangeHandler;
   void fillReferencesToMD(musicDevice::MusicDevice* pMusicDevice);
   void removeReferencesToMD(musicDevice::MusicDevice* pMusicDevice);
};

}   // namespace base::instruments

#include "Instruments.inl"

#endif