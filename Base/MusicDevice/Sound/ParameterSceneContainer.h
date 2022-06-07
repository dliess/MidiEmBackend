#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H

#include <vector>
#include <functional>
#include <memory_resource>

#include "CallbackSignal.h"
#include "ParameterScene.h"
#include "stack_mempool.h"

namespace base::musicDevice::sound
{
class ParameterSceneContainer
{
public:
   ParameterSceneContainer();

   template <typename CB> void forEachActiveModifier(CB&& cb);

   void setSceneName(int sceneIdx, std::string_view name) noexcept;
   void setSceneIntensity(int sceneIdx, float intensity) noexcept;
   void setModifierEndValue(int sceneIdx, const ParameterCoordinate& paramCoord,
                            float value) noexcept;
   void removeModifier(int sceneIdx,
                       const ParameterCoordinate& paramCoord) noexcept;

   CB_SIGNAL(SceneNameChanged, int, const std::string&);
   CB_SIGNAL(SceneIntensityChanged, int, float);
   CB_SIGNAL(ModifierEndValueChanged, int, const ParameterCoordinate&, float);
   CB_SIGNAL(ModifierRemoved, int, const ParameterCoordinate&);

   void retriggerCallbacks() noexcept;

private:
   static constexpr size_t MAX_NUM_SCENES = 64;
   using ContainerT = std::vector<ParameterScene>;
   util::StackMempool<32768> m_memoryPool;
   ContainerT m_data;
};

}   // namespace base::musicDevice::sound

#include "ParameterSceneContainer.inl"

#endif