#include "InstrumentComponentParameterCacheCreator.h"

using namespace base;

std::shared_ptr<instruments::Component::ParameterCache>
instruments::createParameterCache(
    const musicDevice::description::Description* descr, int sdVoiceIdx)
{
   if (descr && descr->soundSection)
   {
      const auto engine = descr->soundSection->engineBase(sdVoiceIdx);
      if (engine)
      {
         auto paramCache =
             std::make_shared<Component::ParameterCache>(engine->parameters.size());
         for (int paramIdx = 0; paramIdx < paramCache->size(); ++paramIdx)
         {
            paramCache->setParameter(
                paramIdx, musicDevice::sound::ParameterAttr::Commanded,
                descr->soundSection->getInitialValueFor(sdVoiceIdx, paramIdx));
         }
         return paramCache;
      }
   }
   return nullptr;
}
/*
InstrumentComponentFactory::InstrumentComponentFactory(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}

Component InstrumentComponentFactory::create(musicDevice::MusicDeviceId soundDeviceId,
                                     int voiceIndex, int noteOffset)
{
    return Component();
}
*/