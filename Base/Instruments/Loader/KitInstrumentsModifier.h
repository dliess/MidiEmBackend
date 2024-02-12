#ifndef INSTRUMENTS_MODIFIER_LOADER_H
#define INSTRUMENTS_MODIFIER_LOADER_H

#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments::loader
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
   void createNewVoiceInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   void addComponentToKitInstrumentVoice(
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

   void insertMelodicInstrument(MelodicInstrument melodicInstrument) noexcept;
   void removeMelodicInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                std::string name) noexcept;

   void createNewVoiceInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;

   void addComponentToMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   void removeComponentFromMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx) noexcept;
   void removeVoiceFromMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept;
   void setNoteOffsetInMelodicInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid,
       int componentIdx, int noteOffset) noexcept;
   void setVoiceNameInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const std::string name) noexcept;

   void setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                               int componentIdx, float amp);

   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);

private:
   Data& m_rData;
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
};

}   // namespace base::instruments::loader
#endif
