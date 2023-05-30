#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_INL

#include "InstrumentComponentParameterCacheFactory.h"

namespace base::instruments
{
template <typename ParameterChangeEmitter>
MelodicInstrumentsParameterCacheCreator<ParameterChangeEmitter>::
    MelodicInstrumentsParameterCacheCreator(
        const MelodicInstrument& melodicInstrument,
        const base::musicDevice::factory::DataHolder& rFactoryDataHolder,
        ParameterChangeEmitter& parameterChangeEmitter) :
    m_rMelodicInstrument(m_rMelodicInstrument),
    m_rFactoryDataHolder(rFactoryDataHolder),
    parameterChangeEmitter(m_rParameterChangeEmitter)
{
}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<
    ParameterChangeEmitter>::createParameterCacheForNextComponentInNextVoice()
{
}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<ParameterChangeEmitter>::
    createParameterCacheForNextComponentInVoice(size_t voiceIdx)
{
    m_rMelodicInstrument.voices().at(voiceIdx)
    return createParameterCache(
                    m_rFactoryDataHolder
                        .getDescription(component.m_soundDeviceId.deviceName())
                        .get(),
                    component.m_sdVoiceIndex)

}

template <typename ParameterChangeEmitter>
std::shared_ptr<Component::ParameterCache>
MelodicInstrumentsParameterCacheCreator<ParameterChangeEmitter>::
    getParameterCacheForComponent(size_t componentIdx)
{
    for(const auto& voice : m_rMelodicInstrument.voices())
    {
        if(voice.components.containsComponent(componentIdx))
        {
            return voice.components[componentIdx].m_pParameterCache;
        }
    }
    return nullptr;
}

}   // namespace base::instruments

#endif
