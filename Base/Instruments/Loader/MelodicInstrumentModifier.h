#ifndef MELODIC_INSTRUMENT_MODIFIER_LOADER_H
#define MELODIC_INSTRUMENT_MODIFIER_LOADER_H

#include "FixedSizeString.h"
#include "InstrumentsData.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"


namespace base::instruments::loader
{
struct MelodicInstrumentModifier
{
   MelodicInstrumentModifier(
       MelodicInstrument& rMelodicInstrument) noexcept;

   Void renameMelodicInstrument(std::string name) noexcept;

   [[nodiscard]] Ret<size_t> findComponentIdxToPlaceNewComponent(
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;
   [[nodiscard]] Ret<size_t>
   findComponentIdxToPlaceNewComponentInVoice(
       int voiceIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;
   
   [[nodiscard]] Ret<std::pair<musicDevice::MusicDeviceName, int>>
   determineComponentEngineType(
      base::musicDevice::factory::DataHolder& rFactoryDataHolder,
      const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;
   
   [[nodiscard]] Ret<std::pair<musicDevice::MusicDeviceName, int>>
   getComponentEngineType(base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const MelodicInstrument& melodicInstrument, size_t componentIdx) const;
   /*
   void createNewVoiceInMelodicInstrument(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx) noexcept;

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
   MelodicInstrument& m_rMelodicInstrument;
   /*
   [[nodiscard]] std::optional<size_t> findComponentIdxToPlaceNewComponent(
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;

   [[nodiscard]] std::optional<size_t>
   findComponentIdxToPlaceNewComponentInVoice(
       int voiceIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;
   [[nodiscard]] std::optional<std::pair<musicDevice::MusicDeviceName, int>>
   determineComponentEngineType(const util::Identifiable::UUID& sdUuid,
                                int sdVoiceIdx) const;
   [[nodiscard]] std::optional<std::pair<musicDevice::MusicDeviceName, int>>
   getComponentEngineType(size_t componentIdx) const;
   */
};

}   // namespace base::instruments::loader
#endif
