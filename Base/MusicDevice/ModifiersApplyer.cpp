#include "ModifiersApplyer.h"

#include "ParameterScene.h"

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
   m_rParameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier &modifier, float intensity) {
          auto mdIter =
              m_rMusicDeviceContainer.find(modifier.destParamCoord.uuid);
          if (mdIter != m_rMusicDeviceContainer.end() &&
              mdIter->second->soundHandler)
          {
             if (modifier.goalValue)
             {
                mdIter->second->soundHandler->applyModifier(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx,
                    modifier.destParamCoord.parameterAttr,
                    modifier.goalValue->value, intensity);
             }
             else
             {
                const auto pv = 
                    mdIter->second->soundHandler->getParameterValue(
                        modifier.destParamCoord.voiceIdx,
                        modifier.destParamCoord.parameterIdx,
                        modifier.destParamCoord.parameterAttr);
                const auto pr = 
                    mdIter->second->soundHandler->getParameterRange(
                        modifier.destParamCoord.voiceIdx,
                        modifier.destParamCoord.parameterIdx,
                        modifier.destParamCoord.parameterAttr);
                modifier.goalValue.emplace(pv, pr);
             }
          }
       });
}
