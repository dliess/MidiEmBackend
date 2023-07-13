#ifndef SOUND_PARAMETER_STORAGE_ELEMENT_H
#define SOUND_PARAMETER_STORAGE_ELEMENT_H

#include <optional>

#include "CallbackSignal.h"
#include "LFO.h"
#include "ParameterAttr.h"
#include "ValueModifier.h"

namespace base::musicDevice::sound
{
struct ParameterStorageElement
{
   ParameterStorageElement(bool isListIndex, int resolution) noexcept;

   [[nodiscard]] std::optional<std::pair<float, float>>
   uiAsksForChangedValues() noexcept;
   void calcActualValueIfLfoActive() noexcept;
   void setCommandedValue(float value, IncrementMode incrementMode) noexcept;
   void incCommandedValue(float increment, IncrementMode incrementMode) noexcept;
   void setValueFromDeviceRel(float value) noexcept;
   void setValueFromDevice(float value) noexcept;
   void applyModifier(float destination, float intensity,
                      ParameterAttr parameterAttr) noexcept;
   void resetModifier(ParameterAttr parameterAttr);
   void calcActualVal();

   void enable(bool enable) noexcept;
   void incUiInterestCount() noexcept;
   void decUiInterestCount() noexcept;
   [[nodiscard]] bool isInSync() const noexcept;
   [[nodiscard]] float commanded() const noexcept;
   [[nodiscard]] const lfo::LFO& lfo() const noexcept;
   [[nodiscard]] lfo::LFO& lfo() noexcept;

   CB_SIGNAL_SINGLE_SUBSCRIBER(ActualChanged, float, float);

private:
   const bool m_isListIndex;
   const int m_resolution;
   float m_cachedLfoValue{0.0};
   float calcModified() const noexcept;

   bool m_enabled{true};
   int m_uiInterestCount{0};
   bool m_dirtyFlagUi{true};
   float m_actual{-1};
   bool m_dirtyFlagRt{false};
   ValueModifier m_modifier;
   float m_commanded{0};
   lfo::LFO m_lfo;
   static constexpr float FUZZ{0.0001f};

   [[nodiscard]] float limitValue(float value,
                                  IncrementMode incrementMode) const noexcept;
   void forceRecalculationAndSending() noexcept;
};

}   // namespace base::musicDevice::sound

#include "SoundParameterStorageElement.inl"

#endif