#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H

#include <list>
#include <memory_resource>
#include <optional>

#include "stack_mempool.h"
#include "ParameterCoordinate.h"

namespace base::musicDevice::sound
{
struct ParameterScene
{
   ParameterScene(std::pmr::unsynchronized_pool_resource& pool) :
       modifiers(&pool)
   {
   }
   std::optional<float> intensity{0};
   struct Modifier
   {
      float goalValue{0};
      ParameterCoordinate destParamCoord;
   };
   std::pmr::list<Modifier> modifiers;
};

}   // namespace base::musicDevice::sound

#endif