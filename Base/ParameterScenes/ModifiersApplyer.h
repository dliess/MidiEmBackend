#ifndef BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H
#define BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H

#include "ParameterSceneContainer.h"
#include "MusicDeviceContainer.h"
#include "CallbackSignal.h"
// clang-format off
namespace base::musicDevice::sound { struct ParameterCoordinate; }
// clang-format on
namespace base::parameterScenes
{
class ModifiersApplyer
{
public:
   ModifiersApplyer(musicDevice::MusicDeviceContainer &rMusicDeviceContainer) noexcept;
   void apply();

   void setSceneName(int sceneIdx, std::string_view name);
   void setSceneIntensity(int sceneIdx, float intensity);
   void setModifierEndValue(int sceneIdx, const musicDevice::sound::ParameterCoordinate& paramCoord,
                            float value);
   void incrementModifierEndValue(int sceneIdx, const musicDevice::sound::ParameterCoordinate& paramCoord,
                            float increment);
   void removeModifier(int sceneIdx,
                       const musicDevice::sound::ParameterCoordinate& paramCoord);

   void retriggerCallbacks();

   CB_SIGNAL(SceneNameChanged, int, const std::string&);
   CB_SIGNAL(SceneIntensityChanged, int, float);
   CB_SIGNAL(ModifierEndValueChanged, int, const musicDevice::sound::ParameterCoordinate&, float);
   CB_SIGNAL(ModifierRemoved, int, const musicDevice::sound::ParameterCoordinate&);

private:
   ParameterSceneContainer m_parameterSceneContainer;
   musicDevice::MusicDeviceContainer &m_rMusicDeviceContainer;
   void clearReferenced();
};

}   // namespace base::musicDevice
#endif