#ifndef SOUND_PARAMETER_STORAGE_ELEMENT_H
#define SOUND_PARAMETER_STORAGE_ELEMENT_H

#include <optional>

#include "LFO.h"
#include "ParameterAttr.h"

namespace base::musicDevice::sound
{
struct ParameterStorageElement
{
   inline ParameterStorageElement(bool isListIndex, int resolution) noexcept;

   [[nodiscard]] inline std::optional<std::pair<float, float>>
   uiAsksForChangedValues() noexcept;
   [[nodiscard]] inline std::optional<std::pair<float, float>>
   updateActualValue() noexcept;
   inline void setCommandedValue(float value, bool roundRobin = false) noexcept;
   inline void incCommandedValue(float increment,
                                 bool roundRobin = false) noexcept;
   inline void setValueFromDeviceRel(float value) noexcept;
   inline void setValueFromDevice(float value) noexcept;
   inline void applyModifier(float destination, float intensity,
                             ParameterAttr parameterAttr) noexcept;

   inline void enable(bool enable) noexcept;
   inline void incUiInterestCount() noexcept;
   inline void decUiInterestCount() noexcept;
   [[nodiscard]] inline bool isInSync() const noexcept;
   [[nodiscard]] inline float commanded() const noexcept;
   [[nodiscard]] inline const lfo::LFO& lfo() const noexcept;
   [[nodiscard]] inline lfo::LFO& lfo() noexcept;

private:
   const bool m_isListIndex;
   const int m_resolution;
   float m_cachedLfoValue{0.0};
   inline float calcModified() const noexcept;

   bool m_enabled{true};
   int m_uiInterestCount{0};
   bool m_dirtyFlagUi{true};
   float m_actual{-1};
   bool m_dirtyFlagRt{false};
   float m_modifier{0};
   float m_commanded{0};
   lfo::LFO m_lfo;
   static constexpr float FUZZ {0.0001};

   [[nodiscard]] inline float limitValue(
       float value, bool roundRobin = false) const noexcept;
   inline void forceRecalculationAndSending() noexcept;
};

}   // namespace base::musicDevice::sound

#include "SoundParameterStorageElement.inl"

#endif