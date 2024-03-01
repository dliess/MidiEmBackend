#ifndef KIT_INSTRUMENT_MODIFIER_RT_H
#define KIT_INSTRUMENT_MODIFIER_RT_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"
#include "MusicDeviceContainer.h"

namespace base::instruments::rt
{
struct KitInstrumentModifier
{
   KitInstrumentModifier(KitInstrument& rKitInstrument) noexcept;

   Void createNewVoiceInKitInstrument(
       base::musicDevice::MusicDeviceContainer& rMDContainer,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void addComponentToKitInstrumentVoice(
       base::musicDevice::MusicDeviceContainer& rMDContainer,
       int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void removeComponentFromKitInstrumentVoice(
       int voiceIdx,
       int componentIdx) noexcept;
   Void removeVoiceFromKitInstrument(
       int voiceIdx) noexcept;
   Void setNoteOffsetInKitInstrumentComponent(
       int voiceIdx,
       int componentIdx, int noteOffset) noexcept;
   Void setNoteOffsetInKitInstrumentVoice(int voiceIdx, int noteOffset) noexcept;
   Void setKitComponentAmp(int voiceIdx, int componentIdx, float amp);
   Void setKitVoiceAmp(int voiceIdx, float amp);

   // Void addVoice(int padIdx, KitVoice voice) noexcept;
   // void fillReferences(base::musicDevice::MusicDevice* pMusicDevice);
   // bool isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const;
private:
   KitInstrument& m_rKitInstrument;
   void forEachComponent(util::function_ref<void(KitComponent&)> f);
};

}   // namespace base::instruments::rt
#endif
