#include "ModifiersApplyer.h"

#include "ParameterScene.h"
#include "SoundHandler.h"

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
            modifier.pCachedSoundHandler = &mdIter->second->soundHandler.value();
             if (modifier.goalValue)
             {
                modifier.pCachedSoundHandler->resetModifier(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx,
                    modifier.destParamCoord.parameterAttr);
             }
          }
       });
   m_rParameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier &modifier, float intensity) {
          if (intensity == 0.0f)
          {
             return;
          }
          if (modifier.pCachedSoundHandler)
          {
             if (modifier.goalValue)
             {
                modifier.pCachedSoundHandler->applyModifier(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx,
                    modifier.destParamCoord.parameterAttr,
                    modifier.goalValue->value, intensity);
             }
             else
             {
                const auto pv = modifier.pCachedSoundHandler->getParameterValue(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx,
                    modifier.destParamCoord.parameterAttr);
                const auto pr =
                    modifier.pCachedSoundHandler->getParameterRangeEnd(
                        modifier.destParamCoord.voiceIdx,
                        modifier.destParamCoord.parameterIdx,
                        modifier.destParamCoord.parameterAttr);
                const float range = R_SWITCH(pr)[](auto &&val)->float
                {
                   return float(val.get());
                }
                R_END_SWITCH
                modifier.goalValue.emplace(*pv, range);
             }
          }
       });
   m_rParameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier &modifier, float intensity) {
          if (modifier.pCachedSoundHandler)
          {
             if (modifier.goalValue)
             {
                modifier.pCachedSoundHandler->calcActualVal(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx);
             }
             modifier.pCachedSoundHandler = nullptr;
          }
       });
   m_rParameterSceneContainer.nulloptZeroIntensityValues();
}
