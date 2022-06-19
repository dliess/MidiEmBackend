#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>
#include "EnumReflect.h"
namespace base::musicDevice::sound::lfo
{

DECLARE_ENUM(Waveform, uint, Sine = 0, Square, Triangle, Saw, Random);

constexpr float DefaultAmplitude = 0.0;
constexpr float DefaultFrequency = 1.0;
constexpr lfo::Waveform DefaultWaveform = lfo::Waveform::Sine;
constexpr int DefaultMultiplierExp = 0;
constexpr uint32_t MAX_MULTIPLIER_EXP = 7;

class LFO
{
public:
   [[nodiscard]] inline bool enabled() const noexcept;
   [[nodiscard]] inline float calculateValue() noexcept;
   inline void setWaveform(Waveform waveform) noexcept;
   inline void setAmplitude(float amplitude) noexcept;
   inline void setFrequency(float frequency) noexcept;
   inline void setMultiplierExp(uint32_t multiplierExp) noexcept;
   inline void applyModifier2Waveform(float destination, float intensity) noexcept;
   inline void applyModifier2Amplitude(float destination, float intensity) noexcept;
   inline void applyModifier2Frequency(float destination, float intensity) noexcept;
   inline void applyModifier2MultiplierExp(float destination, float intensity) noexcept;
   [[nodiscard]] inline Waveform waveform() const noexcept;
   [[nodiscard]] inline float amplitude() const noexcept;
   [[nodiscard]] inline float frequency() const noexcept;
   [[nodiscard]] inline uint32_t multiplierExp() const noexcept;
   inline void reset() noexcept;
   inline bool getAndResetJustGotDisabled() noexcept;
   inline void clearModifiers() noexcept;

   template<typename CB_amp, typename CB_freq, typename CB_waw, typename CB_mult>
   void uiAsksForChanges(CB_amp&& cbAmp, CB_freq&& cbFreq, CB_waw&& cbWaw, CB_mult&& cb_mult);
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
   float m_amplitude{DefaultAmplitude};
   float m_actualAmplitude{DefaultAmplitude};
   float m_frequency{DefaultFrequency};
   float m_actualFrequency{DefaultFrequency};
   uint32_t m_multiplierExp{DefaultMultiplierExp};
   uint32_t m_actualMultiplierExp{DefaultMultiplierExp};
   double m_beatAtWaveStart{0.0};
   using WaveformVariant = mpark::variant<Sine, Square, Triangle, Saw, Random>;
   Waveform m_waveform{Waveform::Sine};
   Waveform m_actualWaveform{Waveform::Sine};
   WaveformVariant m_waveformVariant{Sine{}};

   float m_modifierAmplitude{0};
   float m_modifierFrequency{0};
   float m_modifierMultiplierExp{0};
   float m_modifierWaveform{0};
   [[nodiscard]] inline Waveform modifiedWaveform() const noexcept;
   [[nodiscard]] inline float modifiedAmplitude() const noexcept;
   [[nodiscard]] inline float modifiedFrequency() const noexcept;
   [[nodiscard]] inline uint32_t modifiedMultiplierExp() const noexcept;

   inline void setWaveformVariant(Waveform waveform) noexcept;
   inline void calculateValueMods() noexcept;
   struct DirtyFlags {
      bool amplitude{false};
      bool frequency{false};
      bool waveform{false};
      bool multiplierExp{false};
   };
   DirtyFlags m_dirtyFlagsUi;
};

} // namespace base::musicDevice::sound::lfo

#include "LFO.inl"
#include "LFOMeta.h"

#endif