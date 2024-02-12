#ifndef KIT_INSTRUMENTS_MODIFIER_LOADER_H
#define KIT_INSTRUMENTS_MODIFIER_LOADER_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments::loader
{
struct KitInstrumentsModifier
{
   KitInstrumentsModifier(KitInstruments& rKitInstruments) noexcept;

   void insertKitInstrument(KitInstrument kitInstrument) noexcept;
   void removeKitInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            std::string name) noexcept;
   void createNewVoiceInKitInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   void addComponentToKitInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   void moveKitInstrumentComponent(
       const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
       int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
       int dstVoiceIdx) noexcept;
   void removeComponentFromKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx) noexcept;
   void removeVoiceFromKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept;
   void setNoteOffsetInKitInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx, int noteOffset) noexcept;
   void setNoteOffsetInKitInstrumentVoice(const util::Identifiable::UUID& instrumentUuid,
                                          int voiceIdx, int noteOffset) noexcept;
   void setVoiceNameInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const std::string& name) noexcept;
   void setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                           int componentIdx, float amp);
   void setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp);
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid);

private:
   KitInstruments& m_rKitInstruments;
};

}   // namespace base::instruments::loader
#endif
