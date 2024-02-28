#ifndef BASE_MUSIC_DEVICE_SOUND_VALUE_MODIFIER_H
#define BASE_MUSIC_DEVICE_SOUND_VALUE_MODIFIER_H

namespace base::musicDevice::sound
{

struct ValueModifier
{
   void addAbsoluteModifier(float destination, float intensity);
   void addOffestModifier(float offset, float intensity);
   void clear();

   template<typename T>
   friend T calculateModifiedValue(T commanded, const ValueModifier& modifier);

private:
   float offset_{0};
   float intensity_{0};
};


struct ValueAndModifier
{
   float value;
   ValueModifier modifier;
};

template<typename T>
T calculateModifiedValue(T commanded, const ValueModifier& modifier);

inline
float calculateModifiedValueFloat(const ValueAndModifier& valueAndModifier);

}   // namespace base::musicDevice::sound
#include "ValueModifier.inl"
#endif
