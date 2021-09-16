#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>
#include <random>

namespace base::musicDevice::sound
{
class LFO
{
public:
   enum class Waveform{
      Sine = 0,
      Square,
      Triangle,
      Saw,
      Random
   };
   [[nodiscard]] inline bool enabled() const noexcept;
   [[nodiscard]] inline float calculateValue() noexcept;
   inline bool setWaveform(Waveform waveform) noexcept;
   inline bool setAmplitude(float amplitude) noexcept;
   inline bool setFrequency(float frequency) noexcept;
   [[nodiscard]] inline Waveform waveform() const noexcept;
   [[nodiscard]] inline float amplitude() const noexcept;
   [[nodiscard]] inline float frequency() const noexcept;
   inline void reset() noexcept;
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
      //inline Random() noexcept;
      [[nodiscard]] inline float operator()(float t) noexcept;
      /*
      int lastTime{0};
      float lastValue{0.0};
      std::random_device dev;
      std::mt19937 rng;
      std::uniform_real_distribution<float> dist;
      */
   };

private:
   float m_amplitude{0.0};
   float m_frequency{1.0};
   uint64_t m_lastWaveStartJiffies{0};
   mpark::variant<Sine, Square, Triangle, Saw, Random> m_waveform;
};

} // namespace base::musicDevice::sound

#include "LFO.inl"

#endif