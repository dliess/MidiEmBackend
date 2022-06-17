#ifndef SOUND_PARAMETER_STORAGE_ELEMENT_H
#define SOUND_PARAMETER_STORAGE_ELEMENT_H

#include <optional>

#include "LFO.h"
#include "ParameterPart.h"

namespace base::musicDevice::sound
{

struct ParameterStorageElement
{
   inline ParameterStorageElement(bool isListIndex, int resolution) noexcept;

   bool enabled{true};
   float commanded{0};
   float modifier{0};
   lfo::LFO lfo;
   float actual{-1};
   bool dirtyFlagRt{false};
   bool dirtyFlagUi{true};
   int uiInterestCount{0};

   inline std::optional<std::pair<float, float>>
   uiAsksForChangedValues() noexcept;
   inline std::optional<float> updateActualValue() noexcept;
   inline void setActualValueUnsynced(float value) noexcept;
   inline void setActualValue(float value) noexcept;
   inline void setCommandedValue(float value, bool markDirtyRt = true,
                                 bool roundRobin = false) noexcept;
   inline void incCommandedValue(float increment,
                                 bool roundRobin = false) noexcept;
   inline void applyModifier(float destination, float intensity,
                             ParameterPart parameterPart) noexcept;

private:
   const bool m_isListIndex;
   const int m_resolution;
   float m_cachedLfoValue{0.0};
   inline float calcModified() const noexcept;
};

}   // namespace base::musicDevice::sound

#include "SoundParameterStorageElement.inl"

#endif