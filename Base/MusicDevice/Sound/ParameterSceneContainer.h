#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H

#include <list>
#include <memory_resource>
#include <functional>

#include "CallbackSignal.h"
#include "ParameterScene.h"
#include "stack_mempool.h"

namespace base::musicDevice::sound
{
class ParameterSceneContainer
{
public:
   ParameterSceneContainer();
   using ContainerT = std::pmr::list<ParameterScene>;
   const ContainerT& data() const noexcept { return m_data; }
   ContainerT& data() noexcept { return m_data; }

   template <typename CB>
   void forEachActiveModifier(CB &&cb);

   void setSceneName(const util::Identifiable::UUID& uuid,
                     std::string_view name) noexcept;
   void setSceneIntensity(const util::Identifiable::UUID& uuid,
                          float intensity) noexcept;
   void setModifierEndValue(const util::Identifiable::UUID& sceneUuid,
                            const ParameterCoordinate& paramCoord,
                            float value) noexcept;
   void removeModifier(const util::Identifiable::UUID& sceneUuid,
                       const ParameterCoordinate& paramCoord) noexcept;

   CB_SIGNAL(SceneNameChanged, const util::Identifiable::UUID&,
             const std::string&);
   CB_SIGNAL(SceneIntensityChanged, const util::Identifiable::UUID&, float);
   CB_SIGNAL(ModifierEndValueChanged, const util::Identifiable::UUID&,
             const ParameterCoordinate&, float);
   CB_SIGNAL(ModifierRemoved, const util::Identifiable::UUID&,
             const ParameterCoordinate&);

private:
   util::StackMempool<32768> m_memoryPool;
   ContainerT m_data;
};

}   // namespace base::musicDevice::sound

#include "ParameterSceneContainer.inl"

#endif