#include "InstrumentComponentParameterCacheCreator.h"
#include <exception>

using namespace base;

std::shared_ptr<instruments::ParameterCache>
instruments::createParameterCache(
    const musicDevice::description::Description* descr, int sdVoiceIdx)
{
   if (descr && descr->soundSection)
   {
      const auto engine = descr->soundSection->engineBase(sdVoiceIdx);
      if (engine)
      {
         auto parameterCache =
             std::make_shared<ParameterCache>(engine->parameters.size());
         for (int paramIdx = 0; paramIdx < parameterCache->size(); ++paramIdx)
         {
            parameterCache->setParameterBackup(
                paramIdx, musicDevice::sound::ParameterAttr::Commanded,
                descr->soundSection->getInitialValueFor(sdVoiceIdx, paramIdx));
         }
         parameterCache->syncBackupToRt();
         return parameterCache;
      }
   }
   spdlog::error("createParameterCache failed");
   throw std::runtime_error("ParameterCache creation failed");
}