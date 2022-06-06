#include "ModifiersApplyer.h"

using namespace base::musicDevice;

ModifiersApplyer::ModifiersApplyer(
    sound::ParameterSceneContainer &rParameterSceneContainer,
    MusicDeviceContainer &rMusicDeviceContainer) noexcept :
    m_rParameterSceneContainer(rParameterSceneContainer),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void ModifiersApplyer::operator()() noexcept
{
   for (auto &parameterScene : m_rParameterSceneContainer.data())
   {
      if (parameterScene.intensity)
      {
         for(const auto& modifier : parameterScene.modifiers)
         {
            auto mdIter = m_rMusicDeviceContainer.find(modifier.destParamCoord.uuid);
            if (mdIter != m_rMusicDeviceContainer.end() &&
               mdIter->second->soundHandler)
            {
               mdIter->second->soundHandler->applyModifier(
                  modifier.destParamCoord.voiceIdx,
                  modifier.destParamCoord.parameterIdx,
                  modifier.destParamCoord.parameterPart, modifier.goalValue,
                  parameterScene.intensity.value());
            }
         }
         if(0 == parameterScene.intensity.value())
         {
            parameterScene.intensity.reset();
         }
      }
   }
}
