#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_INL
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_INL

namespace base::musicDevice::sound
{
template <typename CB>
void ParameterSceneContainer::forEachActiveModifier(CB&& cb)
{
   for (auto& scene : m_data)
   {
      if (scene.intensity)
      {
         for (auto& modifier : scene.modifiers) { cb(modifier, scene.intensity.value()); }
         if (0 == scene.intensity.value())
         {
            scene.intensity.reset();
         }
      }
   }
}
}   // namespace base::musicDevice::sound

#endif
