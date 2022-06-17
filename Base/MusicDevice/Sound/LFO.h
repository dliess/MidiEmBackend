#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>
#include "EnumReflect.h"
namespace base::musicDevice::sound::lfo
{

DECLARE_ENUM(Waveform, uint, Sine, Square, Triangle, Saw, Random);

constexpr float DefaultAmplitude = 0.0;
constexpr float DefaultFrequency = 1.0;
constexpr lfo::Waveform DefaultWaveform = lfo::Waveform::Sine;
constexpr int DefaultMultiplierExp = 0;

class LFO
{
public:
   [[nodiscard]] inline bool enabled() const noexcept;
   [[nodiscard]] inline float calculateValue() noexcept;
   inline bool setWaveform(Waveform waveform) noexcept;
   inline bool setAmplitude(float amplitude) noexcept;
   inline bool setFrequency(float frequency) noexcept;
   inline bool setMultiplierExp(uint32_t multiplierExp) noexcept;
   inline void applyModifier2Waveform(float destination, float intensity) noexcept;
   inline void applyModifier2Amplitude(float destination, float intensity) noexcept;
   inline void applyModifier2Frequency(float destination, float intensity) noexcept;
   inline void applyModifier2MultiplierExp(float destination, float intensity) noexcept;
   [[nodiscard]] inline Waveform waveform() const noexcept;
   [[nodiscard]] inline float waveformAsFloat() const noexcept;
   [[nodiscard]] inline float amplitude() const noexcept;
   [[nodiscard]] inline float frequency() const noexcept;
   [[nodiscard]] inline uint32_t multiplierExp() const noexcept;
   [[nodiscard]] inline float multiplierExpAsFloat() const noexcept;
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
   float m_frequency{DefaultFrequency};
   uint32_t m_multiplierExp{DefaultMultiplierExp};
   double m_beatAtWaveStart{0.0};
   using WaveformVariant = mpark::variant<Sine, Square, Triangle, Saw, Random>;
   WaveformVariant m_waveform;
   static constexpr uint32_t MAX_MULTIPLIER_EXP = 7;

   float m_modifierAmplitude{0};
   float m_modifierFrequency{0};
   int m_modifierMultiplierExp{0};
   int m_modifierWaveform{0};
   [[nodiscard]] inline WaveformVariant modifiedWaveform() const noexcept;
   [[nodiscard]] inline float modifiedAmplitude() const noexcept;
   [[nodiscard]] inline float modifiedFrequency() const noexcept;
   [[nodiscard]] inline uint32_t modifiedMultiplierExp() const noexcept;

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