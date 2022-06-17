#include <spdlog/spdlog.h>

#include "SoundParameterStorageElement.h"

namespace base::musicDevice::sound
{

inline ParameterStorageElement::ParameterStorageElement(bool isListIndex,
                                          int resolution) noexcept :
    m_isListIndex(isListIndex), m_resolution(resolution)
{
}

inline std::optional<std::pair<float, float>>
ParameterStorageElement::uiAsksForChangedValues() noexcept
{
   if (dirtyFlagUi && uiInterestCount)
   {
      dirtyFlagUi = false;
      return std::make_pair(commanded, actual);
   }
   return std::nullopt;
}

inline void ParameterStorageElement::applyModifier(
    float destination, float intensity, ParameterPart parameterPart) noexcept
{
   switch (parameterPart)
   {
      case ParameterPart::Commanded:
      {
         modifier += intensity * (destination - commanded);
         break;
      }
      case ParameterPart::LfoAmplitude:
      {
         lfo.applyModifier2Amplitude(destination, intensity);
         break;
      }
      case ParameterPart::LfoFrequency:
      {
         lfo.applyModifier2Frequency(destination, intensity);
         break;
      }
      case ParameterPart::LfoWaveform:
      {
         lfo.applyModifier2Waveform(destination, intensity);
         break;
      }
      case ParameterPart::LfoMultiplierExp:
      {
         lfo.applyModifier2MultiplierExp(destination, intensity);
         break;
      }
   }
   dirtyFlagRt = true;
}

inline std::optional<float>
ParameterStorageElement::updateActualValue() noexcept
{
   if (lfo.getAndResetJustGotDisabled())
      dirtyFlagRt = true;
   if (!enabled || (!dirtyFlagRt && !lfo.enabled()))
   {
      return std::nullopt;
   }
   float actualBefore = actual;
   actual             = commanded + modifier;
   modifier           = 0;
   const float range  = m_isListIndex ? m_resolution : 1.0;
   if (lfo.enabled())
   {
      m_cachedLfoValue = lfo.calculateValue() * range;
      actual += m_cachedLfoValue;
      actual = util::clip(actual, 0.0f, range);
   }
   else
   {
      m_cachedLfoValue = 0.0;
   }

   dirtyFlagRt = false;
   if (m_isListIndex)
   {
      if (int(actualBefore) != int(actual))
      {
         // spdlog::info( "actualBefore {} actual {}", actualBefore, actual);
         dirtyFlagUi = true;
         return actualBefore;
      }
   }
   else
   {
      if (int(actualBefore * m_resolution) != int(actual * m_resolution)
          //|| actual == 0
      )
      {
         dirtyFlagUi = true;
         return actualBefore;
      }
   }
   return std::nullopt;
}

inline void ParameterStorageElement::setActualValueUnsynced(
    float value) noexcept
{
   actual      = value;
   dirtyFlagUi = true;
}

inline void ParameterStorageElement::setActualValue(float value) noexcept
{
   actual      = value;
   commanded   = actual - (m_cachedLfoValue + modifier);
   dirtyFlagUi = true;
}

inline void ParameterStorageElement::setCommandedValue(
    float value, bool markDirtyRt, bool roundRobin) noexcept
{
   const float range = m_isListIndex ? m_resolution : 1.0;
   if (roundRobin)
   {
      if (value < 0.0)
      {
         value = m_isListIndex ? range - 1 : range;
      }
      if (value >= range)
      {
         value = 0.0;
      }
   }
   else
   {
      if (value < 0.0)
      {
         value = 0.0;
      }
      if (value >= range)
      {
         value = m_isListIndex ? range - 1 : range;
      }
   }
   commanded   = value;
   dirtyFlagRt = markDirtyRt;
   dirtyFlagUi = true;
}

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

inline void ParameterStorageElement::incCommandedValue(
    float increment, bool roundRobin) noexcept
{
   const float theIncrement = m_isListIndex ? sgn(increment) : increment;
   setCommandedValue(commanded + theIncrement, true, roundRobin);
}


}   // namespace base::musicDevice::sound
