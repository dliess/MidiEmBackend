#ifndef INSTRUMENTS_INSTRUMENT_VOICE_FACTORY_H
#define INSTRUMENTS_INSTRUMENT_VOICE_FACTORY_H

#include "InstrumentVoice.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::instruments
{
inline std::shared_ptr<Voice::ParameterCache> createParameterCache(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx);

class InstrumentVoiceFactory
{
public:
   InstrumentVoiceFactory(base::musicDevice::factory::DataHolder& rFactoryDataHolder);
   Voice create(musicDevice::MusicDeviceId soundDeviceId, int voiceIndex,
                int noteOffset);
private:
    base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
};

}   // namespace base::instruments

#endif
