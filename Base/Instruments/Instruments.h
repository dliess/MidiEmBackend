#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <functional>
#include <vector>

#include "Identifiable.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "Meta.h"
#include "Settings_old.h"

namespace base
{
namespace musicDevice
{
class MusicDeviceContainer;
}

namespace instruments
{
struct KitInstruments : public std::vector<KitInstrument>
{
   using Super      = std::vector<KitInstrument>;
   KitInstruments() = default;
   KitInstruments(const Super& rhs) : Super(rhs) {}
   Super::iterator findByUUID(const util::Identifiable::UUID& uuid)
   {
      for (auto iter = Super::begin(); iter != Super::end(); ++iter)
         if (iter->id() == uuid)
            return iter;
      return Super::end();
   }
   Super::const_iterator findByUUID(const util::Identifiable::UUID& uuid) const
   {
      for (auto iter = Super::begin(); iter != Super::end(); ++iter)
         if (iter->id() == uuid)
            return iter;
      return Super::end();
   }
};

struct MelodicInstruments : public std::vector<MelodicInstrument>
{
   using Super          = std::vector<MelodicInstrument>;
   MelodicInstruments() = default;
   MelodicInstruments(const Super& rhs) : Super(rhs) {}
   Super::iterator findByUUID(const util::Identifiable::UUID& uuid)
   {
      for (auto iter = Super::begin(); iter != Super::end(); ++iter)
         if (iter->id() == uuid)
            return iter;
      return Super::end();
   }
   Super::const_iterator findByUUID(const util::Identifiable::UUID& uuid) const
   {
      for (auto iter = Super::begin(); iter != Super::end(); ++iter)
         if (iter->id() == uuid)
            return iter;
      return Super::end();
   }
};

struct Instruments : public utils::Settings<Instruments>
{
   Instruments(
       musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept;
   struct Data
   {
      KitInstruments kitInstruments;
      MelodicInstruments melodicInstruments;
   };
   Data data;
   // ============== Settings ===============
   using Settings = Data;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================

   using Cb = std::function<void(void)>;
   void registerForDataChange(Cb cb) noexcept;
   void triggerChanged() noexcept;

   void createKitInstrument(std::string name) noexcept;
   void removeKitInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameKitInstrument(
       const util::Identifiable::UUID& instrumentId,
       std::string name) noexcept;

   void createMelodicInstrument(std::string name) noexcept;
   void removeMelodicInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameMelodicInstrument(
       const util::Identifiable::UUID& instrumentId,
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

private:
   musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
   std::vector<Cb> m_subscribers;
   static constexpr int MAX_VOICES_IN_SLOT = 4;
};

}   // namespace instruments
}   // namespace base

#include "InstrumentsMeta.h"

#endif