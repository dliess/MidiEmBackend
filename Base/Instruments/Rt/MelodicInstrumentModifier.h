#ifndef MELODIC_INSTRUMENT_MODIFIER_RT_H
#define MELODIC_INSTRUMENT_MODIFIER_RT_H

#include "FixedSizeString.h"
#include "InstrumentsData.h"
#include "MelodicInstrument.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"


namespace base::instruments::rt
{
struct MelodicInstrumentModifier
{
   using Engine = base::musicDevice::description::sound::EngineBase;
   MelodicInstrumentModifier(
       MelodicInstrument& rMelodicInstrument) noexcept;

   void renameMelodicInstrument(std::string name) noexcept;
   Void createNewVoiceInMelodicInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void createNewVoiceInMelodicInstrument(
       base::musicDevice::MusicDevice* pMusicDevice, int sdVoiceIdx) noexcept;

   Void addComponentToMelodicInstrumentVoice(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void addComponentToMelodicInstrumentVoice(
       base::musicDevice::MusicDevice* pMusicDevice, int voiceIdx, int sdVoiceIdx) noexcept;
   Void removeComponentFromMelodicInstrumentVoice(
       int voiceIdx,
       int componentIdx) noexcept;
   Void removeVoiceFromMelodicInstrument(int voiceIdx) noexcept;
   Void setNoteOffsetInMelodicInstrumentComponent(
       int componentIdx, int noteOffset) noexcept;

   Void setMelodicComponentAmp(int componentIdx, float amp) noexcept;

   void fillReferences(base::musicDevice::MusicDevice* pMusicDevice);
   bool isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const;
   
private:
   MelodicInstrument& m_rMelodicInstrument;
   [[nodiscard]] Ret<size_t> 
   findComponentIdxToPlaceNewComponent(
       base::musicDevice::MusicDevice* pMusicDevice, int sdVoiceIdx) const;
   
   [[nodiscard]] Ret<MelodicInstrument::ParameterData::EngineId>
   determineComponentEngineId(
      base::musicDevice::MusicDevice* pMusicDevice,
      int sdVoiceIdx) const;
};

}   // namespace base::instruments::rt
#endif
