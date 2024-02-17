#ifndef MELODIC_INSTRUMENTS_MODIFIER_LOADER_H
#define MELODIC_INSTRUMENTS_MODIFIER_LOADER_H

#include "FixedSizeString.h"
#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"


namespace base::instruments::loader
{
struct MelodicInstrumentsModifier
{
   MelodicInstrumentsModifier(
       MelodicInstruments& rMelodicInstruments) noexcept;

   void insertMelodicInstrument(MelodicInstrument melodicInstrument) noexcept;
   Void removeMelodicInstrument(
       const util::Identifiable::UUID& instrumentId) noexcept;
   Void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                std::string name) noexcept;

   Void createNewVoiceInMelodicInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;

   Void addComponentToMelodicInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void removeComponentFromMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx) noexcept;
   Void removeVoiceFromMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept;
   Void setNoteOffsetInMelodicInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid,
       int componentIdx, int noteOffset) noexcept;

   Void setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                               int componentIdx, float amp);

   void fillReferences(base::musicDevice::MusicDevice* pMusicDevice);
   /*
   Void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);
   Void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);
*/
private:
   MelodicInstruments& m_rMelodicInstruments;
   Ret<MelodicInstruments::iterator> getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept;
};

}   // namespace base::instruments::loader
#endif
