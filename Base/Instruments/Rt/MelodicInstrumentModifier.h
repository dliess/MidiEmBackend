#ifndef MELODIC_INSTRUMENT_MODIFIER_RT_H
#define MELODIC_INSTRUMENT_MODIFIER_RT_H

#include "FixedSizeString.h"
#include "InstrumentsData.h"
#include "MelodicInstrument.h"
#include "MusicDeviceContainer.h"
#include "ErrorHandling.h"


namespace base::instruments::rt
{
struct MelodicInstrumentModifier
{
   // using Engine = base::musicDevice::description::sound::EngineBase;
   MelodicInstrumentModifier(
       MelodicInstrument& rMelodicInstrument) noexcept;

   Void createNewVoiceInMelodicInstrument(
       base::musicDevice::MusicDeviceContainer& rMDContainer, int componentIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void createNewVoiceInMelodicInstrument( int componentIdx,
       base::musicDevice::MusicDevice* pMusicDevice, int sdVoiceIdx) noexcept;

   Void addComponentToMelodicInstrumentVoice(
       base::musicDevice::MusicDeviceContainer& rMDContainer,
       int componentIdx,
       int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;
   Void addComponentToMelodicInstrumentVoice(
       int componentIdx, base::musicDevice::MusicDevice* pMusicDevice, int voiceIdx, int sdVoiceIdx) noexcept;
   Void removeComponentFromMelodicInstrumentVoice(
       int voiceIdx,
       int componentIdx) noexcept;
   Void removeVoiceFromMelodicInstrument(int voiceIdx) noexcept;
   Void setNoteOffsetInMelodicInstrumentComponent(
       int componentIdx, int noteOffset) noexcept;

   Void setMelodicComponentAmp(int componentIdx, float amp) noexcept;

   // void fillReferences(base::musicDevice::MusicDevice* pMusicDevice);
   // bool isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const;
   
private:
   MelodicInstrument& m_rMelodicInstrument;
};

}   // namespace base::instruments::rt
#endif
