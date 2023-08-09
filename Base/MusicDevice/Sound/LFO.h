#ifndef BASE_MODULATION_LFO
#define BASE_MODULATION_LFO

#include <Variant.h>

#include "LFOData.h"
#include "ValueModifier.h"
namespace base::musicDevice::sound::lfo
{
enum class DirtyFlags
{
   Empty         = 0,
   Amplitude     = 1,
   Frequency     = 2,
   Waveform      = 4,
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
   [[nodiscard]] bool enabled() const noexcept;
   [[nodiscard]] float calculateValue() noexcept;
   void setWaveform(Waveform waveform) noexcept;
   void setAmplitude(float amplitude) noexcept;
   void setFrequency(float frequency) noexcept;
   void setMultiplierExp(uint32_t multiplierExp) noexcept;
   void applyModifier2Waveform(float destination, float intensity) noexcept;
   void applyModifier2Amplitude(float destination, float intensity) noexcept;
   void applyModifier2Frequency(float destination, float intensity) noexcept;
   void applyModifier2MultiplierExp(float destination,
                                    float intensity) noexcept;
   void clearModifier2Waveform();
   void clearModifier2Amplitude();
   void clearModifier2Frequency();
   void clearModifier2MultiplierExp();

   [[nodiscard]] Waveform waveform() const noexcept;
   [[nodiscard]] float amplitude() const noexcept;
   [[nodiscard]] float frequency() const noexcept;
   [[nodiscard]] uint32_t multiplierExp() const noexcept;

   void reset() noexcept;
   bool getAndResetJustGotDisabled() noexcept;

   template <typename CB_amp, typename CB_freq, typename CB_waw,
             typename CB_mult>
   void uiAsksForChanges(CB_amp&& cbAmp, CB_freq&& cbFreq, CB_waw&& cbWaw,
                         CB_mult&& cb_mult);
   struct Sine
   {
      [[nodiscard]] float operator()(float t) const noexcept;
   };
   struct Square
   {
      [[nodiscard]] float operator()(float t) const noexcept;
      float switchAt{0.5};
   };
   struct Triangle
   {
      [[nodiscard]] float operator()(float t) const noexcept;
   };
   struct Saw
   {
      [[nodiscard]] float operator()(float t) const noexcept;
   };
   struct Random
   {
      [[nodiscard]] float operator()(float t) noexcept;
      float lastValue{0.0};
   };

private:
   bool m_justGotDisabled{false};
   float m_amplitude{DefaultAmplitude};
   float m_actualAmplitude{DefaultAmplitude};
   float m_frequency{DefaultFrequency};
   float m_actualFrequency{DefaultFrequency};
   uint32_t m_multiplierExp{DefaultMultiplierExp};
   uint32_t m_actualMultiplierExp{DefaultMultiplierExp};
   double m_beatAtWaveStart{0.0};
   using WaveformVariant = dl::variant<Sine, Square, Triangle, Saw, Random>;
   Waveform m_waveform{Waveform::Sine};
   Waveform m_actualWaveform{Waveform::Sine};
   WaveformVariant m_waveformVariant{Sine{}};

   ValueModifier m_amplitudeModifier;
   ValueModifier m_frequencyModifier;
   ValueModifier m_multiplierExpModifier;
   ValueModifier m_waveformModifier;
   [[nodiscard]] Waveform modifiedWaveform() const noexcept;
   [[nodiscard]] float modifiedAmplitude() const noexcept;
   [[nodiscard]] float modifiedFrequency() const noexcept;
   [[nodiscard]] uint32_t modifiedMultiplierExp() const noexcept;
   void calcActualVal2Waveform();
   void calcActualVal2Amplitude();
   void calcActualVal2Frequency();
   void calcActualVal2MultiplierExp();

   void setWaveformVariant(Waveform waveform) noexcept;

   DirtyFlags m_dirtyFlagsUi{DirtyFlags::Empty};

};

}   // namespace base::musicDevice::sound::lfo

#include "LFO.inl"

#endif