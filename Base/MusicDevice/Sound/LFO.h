#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>
#include "EnumReflect.h"
namespace base::musicDevice::sound::lfo
{

DECLARE_ENUM(Waveform, uint, Sine, Square, Triangle, Saw, Random);

class LFO
{
public:
   [[nodiscard]] inline bool enabled() const noexcept;
   [[nodiscard]] inline float calculateValue() noexcept;
   inline bool setWaveform(Waveform waveform) noexcept;
   inline bool setAmplitude(float amplitude) noexcept;
   inline bool setFrequency(float frequency) noexcept;
   inline bool setMultiplierExp(uint32_t multiplierExp) noexcept;
   [[nodiscard]] inline Waveform waveform() const noexcept;
   [[nodiscard]] inline float amplitude() const noexcept;
   [[nodiscard]] inline float frequency() const noexcept;
   [[nodiscard]] inline uint32_t multiplierExp() const noexcept;
   inline void reset() noexcept;
   inline bool getAndResetJustGotDisabled() noexcept;
   struct Sine
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
   };
   struct Square
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
      float switchAt{0.5};
   };
   struct Triangle
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
   };
   struct Saw
   {
      [[nodiscard]] inline float operator()(float t) const noexcept;
   };
   struct Random
   {
      [[nodiscard]] inline float operator()(float t) noexcept;
      float lastValue{0.0};
   };

private:
   bool m_justGotDisabled {false};
   float m_amplitude{0.0};
   float m_frequency{1.0};
   uint32_t m_multiplierExp{0};
   uint64_t m_lastWaveStartJiffies{0};
   mpark::variant<Sine, Square, Triangle, Saw, Random> m_waveform;
   static constexpr uint32_t MAX_MULTIPLIER_EXP = 7;
};

} // namespace base::musicDevice::sound::lfo

#include "LFO.inl"

#endif