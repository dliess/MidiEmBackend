#include <spdlog/spdlog.h>

#include "SoundParameterStorageElement.h"

namespace base::musicDevice::sound
{
inline ParameterStorageElement::ParameterStorageElement(bool isListIndex,
                                                        int resolution) noexcept
    :
    m_isListIndex(isListIndex), m_resolution(resolution)
{
}

inline std::optional<std::pair<float, float>>
ParameterStorageElement::uiAsksForChangedValues() noexcept
{
   if (m_dirtyFlagUi && m_uiInterestCount)
   {
      m_dirtyFlagUi = false;
      return std::make_pair(m_commanded, m_actual);
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
         m_modifier += intensity * (destination - m_commanded);
         break;
      }
      case ParameterPart::LfoAmplitude:
      {
         m_lfo.applyModifier2Amplitude(destination, intensity);
         break;
      }
      case ParameterPart::LfoFrequency:
      {
         m_lfo.applyModifier2Frequency(destination, intensity);
         break;
      }
      case ParameterPart::LfoWaveform:
      {
         m_lfo.applyModifier2Waveform(destination, intensity);
         break;
      }
      case ParameterPart::LfoMultiplierExp:
      {
         m_lfo.applyModifier2MultiplierExp(destination, intensity);
         break;
      }
   }
   m_dirtyFlagRt = true;
}

inline std::optional<std::pair<float, float>>
ParameterStorageElement::updateActualValue() noexcept
{
   if (m_lfo.getAndResetJustGotDisabled())
   {
      m_dirtyFlagRt = true;
      m_dirtyFlagUi = true;
   }
   if (!m_enabled || (!m_dirtyFlagRt && !m_lfo.enabled()))
   {
      return std::nullopt;
   }
   float actualBefore = m_actual;
   m_actual           = m_commanded + m_modifier;
   m_modifier         = 0;
   const float range  = m_isListIndex ? m_resolution : 1.0;
   if (m_lfo.enabled())
   {
      m_cachedLfoValue = m_lfo.calculateValue() * range;
      m_lfo.clearModifiers();
      m_actual += m_cachedLfoValue;
      m_actual = limitValue(m_actual);
   }
   else
   {
      m_cachedLfoValue = 0.0;
   }

   m_dirtyFlagRt = false;
   if (m_isListIndex)
   {
      actualBefore = int(actualBefore);
      m_actual = int(m_actual);
      if (int(actualBefore) != int(m_actual))
      {
         m_dirtyFlagUi = true;
         return std::make_pair(actualBefore, m_actual);
      }
   }
   else
   {
      if (int(actualBefore * m_resolution) != int(m_actual * m_resolution))
      {
         m_dirtyFlagUi = true;
         return std::make_pair(actualBefore, m_actual);
      }
   }
   return std::nullopt;
}

inline void ParameterStorageElement::setCommandedValue(float value,
                                                       bool roundRobin) noexcept
{
   m_commanded   = limitValue(value, roundRobin);
   m_dirtyFlagRt = true;
   m_dirtyFlagUi = true;
}

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

inline void ParameterStorageElement::incCommandedValue(float increment,
                                                       bool roundRobin) noexcept
{
   const float theIncrement = m_isListIndex ? sgn(increment) : increment;
   setCommandedValue(m_commanded + theIncrement, roundRobin);
}

inline void ParameterStorageElement::setValueFromDeviceRel(float value) noexcept
{
   m_actual      = value;
   m_commanded   = limitValue(m_actual - (m_cachedLfoValue + m_modifier));
   m_dirtyFlagUi = true;
}

inline void ParameterStorageElement::setValueFromDevice(float value) noexcept
{
   m_commanded   = limitValue(value);
   m_actual      = m_commanded;
   m_dirtyFlagRt = true;
   m_dirtyFlagUi = true;
}

inline float ParameterStorageElement::limitValue(float value,
                                                 bool roundRobin) const noexcept
{
   float ret = value;
   const float range = m_isListIndex ? m_resolution : 1.0;
   if (roundRobin)
   {
      if (value < 0.0)
      {
         ret = m_isListIndex ? range - 1 : range - FUZZ;
      }
      if (value >= range)
      {
         ret = 0.0;
      }
   }
   else
   {
      if (value < 0.0)
      {
         ret = 0.0;
      }
      if (value >= range)
      {
         ret = m_isListIndex ? range - 1 : range - FUZZ;
      }
   }
   return ret;
}

inline void ParameterStorageElement::enable(bool enable) noexcept
{
   if (m_enabled != enable)
   {
      m_enabled = enable;
      if (m_enabled)
      {
         forceRecalculationAndSending();
      }
   }
}

inline void ParameterStorageElement::incUiInterestCount() noexcept
{
   m_dirtyFlagUi = true;
   ++m_uiInterestCount;
}

inline void ParameterStorageElement::decUiInterestCount() noexcept
{
   --m_uiInterestCount;
   if (m_uiInterestCount < 0)
      m_uiInterestCount = 0;
}

inline void ParameterStorageElement::forceRecalculationAndSending() noexcept
{
   m_actual      = -1;
   m_dirtyFlagRt = true;
}

inline bool ParameterStorageElement::isInSync() const noexcept
{
   return m_actual != -1;
}

inline float ParameterStorageElement::commanded() const noexcept
{
   return m_commanded;
}

inline const lfo::LFO& ParameterStorageElement::lfo() const noexcept
{
   return m_lfo;
}

inline lfo::LFO& ParameterStorageElement::lfo() noexcept { return m_lfo; }

}   // namespace base::musicDevice::sound
