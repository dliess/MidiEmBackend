#ifndef KIT_INSTRUMENTS_MODIFIER_LOADER_H
#define KIT_INSTRUMENTS_MODIFIER_LOADER_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"

namespace base::instruments::loader
{
struct KitInstrumentsModifier
{
   KitInstrumentsModifier(KitInstruments& rKitInstruments) noexcept;

   void insertKitInstrument(KitInstrument kitInstrument) noexcept;
   Void removeKitInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   Void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            std::string name) noexcept;
   Void createNewVoiceInKitInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void addComponentToKitInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void moveKitInstrumentComponent(
       const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
       int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
       int dstVoiceIdx) noexcept;
   Void removeComponentFromKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx) noexcept;
   Void removeVoiceFromKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept;
   Void setNoteOffsetInKitInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx, int noteOffset) noexcept;
   Void setNoteOffsetInKitInstrumentVoice(const util::Identifiable::UUID& instrumentUuid,
                                          int voiceIdx, int noteOffset) noexcept;
   Void setVoiceNameInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const std::string& name) noexcept;
   Void setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                           int componentIdx, float amp);
   Void setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp);
   void fillReferences(base::musicDevice::MusicDevice* pMusicDevice);
   // void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   // void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid);

private:
   KitInstruments& m_rKitInstruments;
   Ret<KitInstruments::iterator> getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept;
};

}   // namespace base::instruments::loader
#endif
