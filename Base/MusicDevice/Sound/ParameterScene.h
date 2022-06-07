#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H

#include <list>
#include <memory_resource>
#include <optional>

#include "Identifiable.h"
#include "stack_mempool.h"
#include "ParameterCoordinate.h"

namespace base::musicDevice::sound
{
struct ParameterScene : public util::Identifiable
{
   ParameterScene(std::pmr::unsynchronized_pool_resource& pool) :
       name(&pool),
       modifiers(&pool)
   {
   }
   ParameterScene(std::string_view _name, std::pmr::unsynchronized_pool_resource& pool) :
       name(_name, &pool),
       modifiers(&pool)
   {
   }
   std::pmr::string name;
   std::optional<float> intensity{0};
   struct Modifier
   {
      Modifier(const ParameterCoordinate& pc, float val) noexcept: destParamCoord(pc), goalValue(val) {}
      ParameterCoordinate destParamCoord;
      float goalValue{0};
   };
   std::pmr::list<Modifier> modifiers;
};

}   // namespace base::musicDevice::sound

#endif