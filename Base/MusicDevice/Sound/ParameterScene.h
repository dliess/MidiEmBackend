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
      Modifier(const ParameterCoordinate& pc, float val, float range) noexcept: destParamCoord(pc), goalValue({val, range}) {}
      Modifier(const ParameterCoordinate& pc) noexcept: destParamCoord(pc), goalValue() {}
      ParameterCoordinate destParamCoord;
      struct GoalValue{
        GoalValue(float _value, float _range) noexcept : value(_value), range(_range) {}
        float value{0};
        float range{1.0};
      };
      std::optional<GoalValue> goalValue;
   };
   std::pmr::list<Modifier> modifiers;
};

}   // namespace base::musicDevice::sound

#endif