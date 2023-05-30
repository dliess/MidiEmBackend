#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_H

#include "InstrumentComponent.h"
#include "MelodicInstrument.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments
{
std::shared_ptr<Component::ParameterCache> createParameterCache(
    const musicDevice::description::Description* pDescription, int sdVoiceIdx);

template <typename ParameterChangeEmitter>
class MelodicInstrumentsParameterCacheCreator
{
public:
   explicit MelodicInstrumentsParameterCacheCreator(
       const MelodicInstrument& melodicInstrument,
       const base::musicDevice::factory::DataHolder& rFactoryDataHolder,
       ParameterChangeEmitter& parameterChangeEmitter);

   std::shared_ptr<Component::ParameterCache>
   createParameterCacheForNextComponentInNextVoice();
   std::shared_ptr<Component::ParameterCache>
   createParameterCacheForNextComponentInVoice(size_t voiceIdx);
   std::shared_ptr<Component::ParameterCache> getParameterCacheForComponent(
       size_t componentIdx);

private:
   const MelodicInstrument& m_rMelodicInstrument;
   const base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   ParameterChangeEmitter& m_rParameterChangeEmitter;
};

}   // namespace base::instruments

#include "InstrumentComponentParameterCacheFactory.inl"

#endif
