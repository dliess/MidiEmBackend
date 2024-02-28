#ifndef BASE_MUSIC_DEVICE_SOUND_VALUE_MODIFIER_INL
#define BASE_MUSIC_DEVICE_SOUND_VALUE_MODIFIER_INL

#include "ValueModifier.h"
#include "toIntegral.h"

namespace base::musicDevice::sound
{

inline
void ValueModifier::addAbsoluteModifier(float destination, float intensity)
{
   offset_ += destination * intensity;
   intensity_ += intensity; 
}

inline
void ValueModifier::addOffestModifier(float offset, float intensity)
{
   offset_ += offset * intensity;
}

template<typename T>
concept EnumClass = std::is_enum_v<T> && !std::is_convertible_v<T, int>;

template<typename T>
T calculateModifiedValue(T commanded, const ValueModifier& modifier)
{
   if constexpr (EnumClass<T>)
   {
      const auto c = util::to_integral(commanded);
      return T(c + modifier.offset_ - (c * modifier.intensity_));      
   }
   else
   {
      return commanded + modifier.offset_ - (commanded * modifier.intensity_); 
   }
}

inline
float calculateModifiedValueFloat(const ValueAndModifier& valueAndModifier)
{
   return calculateModifiedValue(valueAndModifier.value, valueAndModifier.modifier);
}

inline
void ValueModifier::clear()
{
   offset_ = 0.0f;
   intensity_ = 0.0f;
}


}   // namespace base::musicDevice::sound

#endif
