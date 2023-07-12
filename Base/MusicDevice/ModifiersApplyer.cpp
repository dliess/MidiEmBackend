#include "ModifiersApplyer.h"

#include "ParameterScene.h"
#include "SoundHandler.h"

using namespace base::musicDevice;

ModifiersApplyer::ModifiersApplyer(
    MusicDeviceContainer& rMusicDeviceContainer) noexcept :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
   m_parameterSceneContainer.onSceneNameChanged(
       [this](int sceneIdx, const std::string& name) {
          emitSceneNameChanged(sceneIdx, name);
       });
   m_parameterSceneContainer.onSceneIntensityChanged(
       [this](int sceneIdx, float intensity) {
          emitSceneIntensityChanged(sceneIdx, intensity);
       });
   m_parameterSceneContainer.onModifierEndValueChanged(
       [this](int sceneIdx, const sound::ParameterCoordinate& coord,
              float value) {
          emitModifierEndValueChanged(sceneIdx, coord, value);
       });
   m_parameterSceneContainer.onModifierRemoved(
       [this](int sceneIdx, const sound::ParameterCoordinate& coord) {
          emitModifierRemoved(sceneIdx, coord);
       });
}

void ModifiersApplyer::setSceneName(int sceneIdx, std::string_view name)
{
   m_parameterSceneContainer.setSceneName(sceneIdx, name);
}

void ModifiersApplyer::setSceneIntensity(int sceneIdx, float intensity)
{
   clearReferenced();
   m_parameterSceneContainer.setSceneIntensity(sceneIdx, intensity);
   apply();
}

void ModifiersApplyer::setModifierEndValue(
    int sceneIdx, const sound::ParameterCoordinate& paramCoord, float value)
{
   clearReferenced();
   m_parameterSceneContainer.setModifierEndValue(sceneIdx, paramCoord, value);
   apply();
}

void ModifiersApplyer::incrementModifierEndValue(
    int sceneIdx, const sound::ParameterCoordinate& paramCoord, float increment)
{
   clearReferenced();
   m_parameterSceneContainer.incrementModifierEndValue(sceneIdx, paramCoord, increment);
   apply();
}

void ModifiersApplyer::removeModifier(
    int sceneIdx, const sound::ParameterCoordinate& paramCoord)
{
   clearReferenced();
   m_parameterSceneContainer.removeModifier(sceneIdx, paramCoord);
   apply();
}

void ModifiersApplyer::retriggerCallbacks()
{
   m_parameterSceneContainer.retriggerCallbacks();
}

void ModifiersApplyer::clearReferenced()
{
   m_parameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier& modifier, float intensity) {
          auto mdIter =
              m_rMusicDeviceContainer.find(modifier.destParamCoord.uuid);
          if (mdIter != m_rMusicDeviceContainer.end() &&
              mdIter->second->soundHandler)
          {
             modifier.pCachedSoundHandler =
                 &mdIter->second->soundHandler.value();
             if (modifier.goalValue)
             {
                modifier.pCachedSoundHandler->resetModifier(
                    modifier.destParamCoord.voiceIdx,
                    modifier.destParamCoord.parameterIdx,
                    modifier.destParamCoord.parameterAttr);
             }
          }
       });
}

void ModifiersApplyer::apply()
{
   m_parameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier& modifier, float intensity) {
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
                const float range = R_SWITCH(pr)[](auto&& val)->float
                {
                   return float(val.get());
                }
                R_END_SWITCH
                modifier.goalValue.emplace(*pv, range);
             }
          }
       });
   m_parameterSceneContainer.forEachActiveModifier(
       [this](sound::ParameterScene::Modifier& modifier, float) {
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
   m_parameterSceneContainer.nulloptZeroIntensityValues();
}
