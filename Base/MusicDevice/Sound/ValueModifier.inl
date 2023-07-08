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
T calculateModifiedValue(T commanded, const ValueModifier& modifier)
{
   return commanded + modifier.offset_ - (commanded * modifier.intensity_); 
}

template<> inline
lfo::Waveform calculateModifiedValue(lfo::Waveform commanded, const ValueModifier& modifier)
{
   const auto c = util::to_integral(commanded);
   return lfo::Waveform(c + modifier.offset_ - (c * modifier.intensity_)); 
}

inline
void ValueModifier::clear()
{
   offset_ = 0.0f;
   intensity_ = 0.0f;
}


}   // namespace base::musicDevice::sound

#endif
