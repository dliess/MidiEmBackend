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
   /*

   void addComponentToMelodicInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
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
*/
private:
   MelodicInstruments& m_rMelodicInstruments;
   Ret<MelodicInstruments::iterator> getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept;
};

}   // namespace base::instruments::loader
#endif
