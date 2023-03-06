#include "InstrumentVoiceFactory.h"

using namespace base::instruments;

std::shared_ptr<Voice::ParameterCache> createParameterCache(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx)
{
   auto descr = rFactoryDataHolder.getDescription(soundDeviceUuid);
   if (descr && descr->soundSection)
   {
      const auto engine = descr->soundSection->engineBase(voiceIdx);
      if (engine)
      {
         auto paramCache =
             std::make_shared<Voice::ParameterCache>(engine->parameters.size());
         for (int paramIdx = 0; paramIdx < paramCache->size(); ++paramIdx)
         {
            paramCache->at(paramIdx).commanded =
                descr->soundSection->getInitialValueFor(voiceIdx, paramIdx);
         }
         return paramCache;
      }
   }
   return nullptr;
}

InstrumentVoiceFactory::InstrumentVoiceFactory(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}

Voice InstrumentVoiceFactory::create(musicDevice::MusicDeviceId soundDeviceId,
                                     int voiceIndex, int noteOffset)
{
    return Voice();
}
