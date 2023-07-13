#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_H

#include <list>
#include <memory_resource>
#include <optional>

#include "ParameterCoordinate.h"
#include "stack_mempool.h"

// clang-format off
namespace base::musicDevice::sound { class SoundHandler; }
// clang-format on

namespace base::parameterScenes
{
struct ParameterScene
{
   explicit ParameterScene(std::pmr::unsynchronized_pool_resource& pool) :
       name(&pool), modifiers(&pool)
   {
   }
   ParameterScene(std::string_view _name,
                  std::pmr::unsynchronized_pool_resource& pool) :
       name(_name, &pool), modifiers(&pool)
   {
   }
   std::pmr::string name;
   std::optional<float> intensity{0};
   struct Modifier
   {
      explicit Modifier(const musicDevice::sound::ParameterCoordinate& pc, float val,
               float range) noexcept :
          destParamCoord(pc), goalValue({val, range})
      {
      }
      explicit Modifier(const musicDevice::sound::ParameterCoordinate& pc) noexcept :
          destParamCoord(pc), goalValue()
      {
      }
      musicDevice::sound::ParameterCoordinate destParamCoord;
      struct GoalValue
      {
         GoalValue(float _value, float _range) noexcept :
             value(_value), range(_range)
         {
         }
         float value{0};
         float range{1.0};
      };
      std::optional<GoalValue> goalValue;
      musicDevice::sound::SoundHandler* pCachedSoundHandler{nullptr};
   };
   std::pmr::list<Modifier> modifiers;
};

}   // namespace base::parameterScenes

#endif