#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <mpark/variant.hpp>
#include "LFOData.h"
#include "ValueModifier.h"
namespace base::musicDevice::sound::lfo
{

enum class DirtyFlags {
   Empty = 0,
   Amplitude = 1,
   Frequency = 2,
   Waveform = 4,
   MultiplierExp = 8
};

inline DirtyFlags operator|(DirtyFlags a, DirtyFlags b)
{
    return static_cast<DirtyFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline DirtyFlags& operator|=(DirtyFlags& a, DirtyFlags b)
{
   a = a | b;
   return a; 
}
inline int operator & (DirtyFlags a, DirtyFlags b)
{
   return static_cast<int>(a) & static_cast<int>(b);
}

class LFO
{
public:
   inline void calculateValueMods() noexcept;
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
   [[nodiscard]] inline bool dirty() const noexcept;

   inline void reset() noexcept;
   inline bool getAndResetJustGotDisabled() noexcept;

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

   ValueModifier m_amplitudeModifier;
   ValueModifier m_frequencyModifier;
   ValueModifier m_multiplierExpModifier;
   ValueModifier m_waveformModifier;
   [[nodiscard]] inline Waveform modifiedWaveform() const noexcept;
   [[nodiscard]] inline float modifiedAmplitude() const noexcept;
   [[nodiscard]] inline float modifiedFrequency() const noexcept;
   [[nodiscard]] inline uint32_t modifiedMultiplierExp() const noexcept;

   inline void setWaveformVariant(Waveform waveform) noexcept;
   inline void clearModifiers() noexcept;

   DirtyFlags m_dirtyFlagsUi{DirtyFlags::Empty};
};

} // namespace base::musicDevice::sound::lfo

#include "LFO.inl"

#endif