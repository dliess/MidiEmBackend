#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_LOADER_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_PARAMETER_CACHE_CREATOR_LOADER_H

#include "InstrumentComponent.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments::loader
{

class MelodicInstrumentsParameterCacheCreator
{
public:
   explicit MelodicInstrumentsParameterCacheCreator(
       musicDevice::factory::DataHolder& rFactoryDataHolder);


   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForNextMatchingComponentInNextVoice(
       const MelodicInstrument& rMelodicInstrument,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;

   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForNextMatchingComponentInVoice(
       const MelodicInstrument& rMelodicInstrument, int voiceIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;

   [[nodiscard]] std::optional<size_t> findComponentIdxToPlaceNewComponent(
       const MelodicInstrument& melodicInstrument,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;

   [[nodiscard]] std::optional<size_t>
   findComponentIdxToPlaceNewComponentInVoice(
       const MelodicInstrument& melodicInstrument, int voiceIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) const;
private:
   musicDevice::factory::DataHolder& m_rFactoryDataHolder;

   [[nodiscard]] std::shared_ptr<ParameterCache>
   getParameterCacheForComponent(const MelodicInstrument& rMelodicInstrument,
                                 size_t componentIdx) const;
   // TODO: strong type engineIdx
   [[nodiscard]] std::optional<std::pair<musicDevice::MusicDeviceName, int>>
   getComponentEngineType(const MelodicInstrument& rMelodicInstrument,
                          size_t componentIdx) const;

   [[nodiscard]] std::optional<std::pair<musicDevice::MusicDeviceName, int>>
   determineComponentEngineType(const util::Identifiable::UUID& sdUuid,
                                int sdVoiceIdx) const;



   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForComponent(
       const MelodicInstrument& rMelodicInstrument, int componentIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;
};

class KitInstrumentsParameterCacheCreator
{
public:
   explicit KitInstrumentsParameterCacheCreator(
       musicDevice::factory::DataHolder& rFactoryDataHolder);

   template <typename ParameterChangeEmitter>
   void initParameterCaches(KitInstrument& kitInstrument,
                            ParameterChangeEmitter& rParameterChangeEmitter);

   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForNewComponentInNextVoice(
       const KitInstrument& kitInstrument,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;

   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForNewComponentInVoice(
       const KitInstrument& kitInstrument, int voiceIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;

private:
   musicDevice::factory::DataHolder& m_rFactoryDataHolder;

   template <typename ParameterChangeEmitter>
   [[nodiscard]] std::shared_ptr<ParameterCache>
   createParameterCacheForComponent(
       const KitInstrument& kitInstrument, int voiceIdx, int componentIdx,
       const util::Identifiable::UUID& sdUuid, int sdVoiceIdx,
       ParameterChangeEmitter& rParameterChangeEmitter) const;
};

}   // namespace base::instruments::loader

#include "InstrumentComponentParameterCacheCreator.inl"

#endif
