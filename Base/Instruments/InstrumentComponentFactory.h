#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_FACTORY_H

#include "InstrumentComponent.h"
#include "MusicDeviceDescription.h"

namespace base::instruments
{
std::shared_ptr<Component::ParameterCache> createParameterCache(
    const musicDevice::description::Description* pDescription, int sdVoiceIdx);

/*
class InstrumentComponentFactory
{
public:
   InstrumentComponentFactory(base::musicDevice::factory::DataHolder&
rFactoryDataHolder); Component create(musicDevice::MusicDeviceId soundDeviceId, int
voiceIndex, int noteOffset); private: base::musicDevice::factory::DataHolder&
m_rFactoryDataHolder;
};
*/
}   // namespace base::instruments

#endif
