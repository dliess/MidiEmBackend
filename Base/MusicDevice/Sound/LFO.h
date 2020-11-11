#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>

namespace base::musicDevice::sound
{
class LFO
{
public:
   [[nodiscard]] inline bool enabled() const noexcept;
   [[nodiscard]] inline float calculateValue() const noexcept;
   inline void setAmplitude(float amplitude) noexcept;
   inline void setFrequency(float frequency) noexcept;
   struct Sine
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
   };
   struct Square
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
      float switchAt{0.5};
      bool inverted{false};
   };

private:
   float m_amplitude{0.0};
   float m_frequency{0.0};
   mpark::variant<Sine, Square> m_waveform;
};

} // namespace base::musicDevice::sound

#include "LFO.inl"

#endif