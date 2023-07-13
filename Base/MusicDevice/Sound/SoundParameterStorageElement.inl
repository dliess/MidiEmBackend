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
    float destination, float intensity, ParameterAttr parameterAttr) noexcept
{
   switch (parameterAttr)
   {
      case ParameterAttr::Commanded:
      {
         m_modifier.addAbsoluteModifier(destination, intensity);
         break;
      }
      case ParameterAttr::LfoFrequency:
      {
         m_lfo.applyModifier2Frequency(destination, intensity);
         break;
      }
      case ParameterAttr::LfoAmplitude:
      {
         m_lfo.applyModifier2Amplitude(destination, intensity);
         break;
      }
      case ParameterAttr::LfoWaveform:
      {
         m_lfo.applyModifier2Waveform(destination, intensity);
         break;
      }
      case ParameterAttr::LfoMultiplierExp:
      {
         m_lfo.applyModifier2MultiplierExp(destination, intensity);
         break;
      }
   }
   m_dirtyFlagRt = true;
}

inline void ParameterStorageElement::resetModifier(ParameterAttr parameterAttr)
{
   switch (parameterAttr)
   {
      case ParameterAttr::Commanded:
      {
         m_modifier.clear();
         break;
      }
      case ParameterAttr::LfoFrequency:
      {
         m_lfo.clearModifier2Frequency();
         break;
      }
      case ParameterAttr::LfoAmplitude:
      {
         m_lfo.clearModifier2Amplitude();
         break;
      }
      case ParameterAttr::LfoWaveform:
      {
         m_lfo.clearModifier2Waveform();
         break;
      }
      case ParameterAttr::LfoMultiplierExp:
      {
         m_lfo.clearModifier2MultiplierExp();
         break;
      }
   }
   m_dirtyFlagRt = true;
}

inline void ParameterStorageElement::calcActualVal()
{
   if(!m_dirtyFlagRt || !m_enabled)
   {
      return;
   }
   float actualBefore = m_actual;
   m_actual           = calculateModifiedValue(m_commanded, m_modifier);
   const float range  = m_isListIndex ? m_resolution : 1.0;
   if (m_lfo.enabled())
   {
      m_cachedLfoValue = m_lfo.calculateValue() * range;
      m_actual += m_cachedLfoValue;
   }
   else
   {
      m_cachedLfoValue = 0.0;
   }
   m_actual = limitValue(m_actual, IncrementMode::Limit);
   m_dirtyFlagRt = false;
   if (m_isListIndex)
   {
      actualBefore = int(actualBefore);
      m_actual = int(m_actual);
      if (int(actualBefore) != int(m_actual))
      {
         m_dirtyFlagUi = true;
         emitActualChanged(actualBefore, m_actual);
      }
   }
   else
   {
      if (int(actualBefore * m_resolution) != int(m_actual * m_resolution))
      {
         m_dirtyFlagUi = true;
         emitActualChanged(actualBefore, m_actual);
      }
   }
}


inline void
ParameterStorageElement::calcActualValueIfLfoActive() noexcept
{
   if (m_lfo.getAndResetJustGotDisabled() || m_lfo.enabled())
   {
      m_dirtyFlagRt = true;
      m_dirtyFlagUi = true;
   }
   calcActualVal();
}

inline void ParameterStorageElement::setCommandedValue(float value,
                                                       IncrementMode incrementMode) noexcept
{
   m_commanded   = limitValue(value, incrementMode);
   m_dirtyFlagRt = true;
   m_dirtyFlagUi = true;

}

template <typename T> int sgn(T val) { return int(T(0) < val) - int(val < T(0)); }

inline void ParameterStorageElement::incCommandedValue(float increment,
                                                       IncrementMode incrementMode) noexcept
{
   const float theIncrement = m_isListIndex ? float(sgn(increment)) : increment;
   setCommandedValue(m_commanded + theIncrement, incrementMode);
}

inline void ParameterStorageElement::setValueFromDeviceRel(float value) noexcept
{
   // TODO this impl. is bad, it doesnt count in the modifier
   m_actual      = value;
   m_commanded   = limitValue(m_actual - (m_cachedLfoValue), IncrementMode::Limit);
   m_dirtyFlagUi = true;
}

inline void ParameterStorageElement::setValueFromDevice(float value) noexcept
{
   m_commanded   = limitValue(value, IncrementMode::Limit);
   m_actual      = m_commanded;
   m_dirtyFlagRt = true;
   m_dirtyFlagUi = true;
}

inline float ParameterStorageElement::limitValue(float value,
                                                 IncrementMode incrementMode) const noexcept
{
   float ret = value;
   const float range = m_isListIndex ? m_resolution : 1.0;
   if (incrementMode == IncrementMode::RoundRobin)
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

inline lfo::LFO& ParameterStorageElement::lfo() noexcept
{ 
   m_dirtyFlagRt = true;
   return m_lfo;
}

}   // namespace base::musicDevice::sound
