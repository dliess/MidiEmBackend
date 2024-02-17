#ifndef KIT_INSTRUMENT_MODIFIER_LOADER_H
#define KIT_INSTRUMENT_MODIFIER_LOADER_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"

namespace base::instruments::loader
{
struct KitInstrumentModifier
{
   KitInstrumentModifier(KitInstrument& rKitInstrument) noexcept;

   void renameKitInstrument(std::string name) noexcept;
   Void createNewVoiceInKitInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void addComponentToKitInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
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
   Void setVoiceNameInKitInstrument(int voiceIdx,
       const std::string& name) noexcept;
   Void setKitComponentAmp(int voiceIdx, int componentIdx, float amp);
   Void setKitVoiceAmp(int voiceIdx, float amp);
   //void incKitInstrumentRefCount();
   //void decKitInstrumentRefCount();

   Void addVoice(int padIdx, KitVoice voice) noexcept;
private:
   KitInstrument& m_rKitInstrument;
};

}   // namespace base::instruments::loader
#endif
