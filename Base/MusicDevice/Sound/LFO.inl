#include <cmath>
#include <random>

#include "BeatTick.h"
#include "LFO.h"
#include "Overload.h"
#include "clip.h"

namespace base::musicDevice::sound::lfo
{
inline bool LFO::enabled() const noexcept
{
   return (modifiedAmplitude() != 0.0) && (modifiedFrequency() != 0.0);
}

inline float LFO::calculateValue() noexcept
{
   const auto beat = tempo::BeatTick::instance().getBeat();
   auto deltaBeat  = beat - m_beatAtWaveStart;
   const auto period =
       1.0 / (modifiedFrequency() * (1 << modifiedMultiplierExp()));
   if (deltaBeat >= period)
   {
      m_beatAtWaveStart = beat;
      deltaBeat         = 0;
   }
   const auto t = deltaBeat / period;
   const auto fnVal =
       mpark::visit(util::overload{[t](auto&& f) { return f(t); }},
                    m_waveform);   // TODO: waveform modification does not work
   return modifiedAmplitude() * fnVal;
};

inline bool LFO::setWaveform(Waveform waveform) noexcept
{
   switch (waveform)
   {
      case Waveform::Sine: m_waveform.emplace<Sine>(); return true;
      case Waveform::Square: m_waveform.emplace<Square>(); return true;
      case Waveform::Triangle: m_waveform.emplace<Triangle>(); return true;
      case Waveform::Saw: m_waveform.emplace<Saw>(); return true;
      case Waveform::Random: m_waveform.emplace<Random>(); return true;
   }
   return false;
}

inline bool LFO::setAmplitude(float amplitude) noexcept
{
   if (m_amplitude != amplitude && amplitude >= -1.0 && amplitude <= 1.0)
   {
      m_amplitude = amplitude;
      if (m_amplitude == 0.0)
         m_justGotDisabled = true;
      return true;
   }
   return false;
}

inline bool LFO::setFrequency(float frequency) noexcept
{
   if (frequency < 0.0)
      frequency = 0.0;
   if (m_frequency != frequency && frequency >= 0.0 && frequency <= 1.0)
   {
      m_frequency = frequency;
      if (m_frequency == 0.0)
         m_justGotDisabled = true;
      return true;
   }
   return false;
}

inline bool LFO::setMultiplierExp(uint32_t multiplierExp) noexcept
{
   if (m_multiplierExp != multiplierExp && multiplierExp <= MAX_MULTIPLIER_EXP)
   {
      m_multiplierExp = multiplierExp;
      return true;
   }
   return false;
}

inline void LFO::applyModifier2Waveform(float destination,
                                        float intensity) noexcept
{
   m_modifierWaveform +=
       ((destination *
         mpark::variant_size_v<decltype(m_waveform)>)-m_waveform.index()) *
       intensity;
}

inline void LFO::applyModifier2Amplitude(float destination,
                                         float intensity) noexcept
{
   m_modifierAmplitude += (destination - m_amplitude) * intensity;
}

inline void LFO::applyModifier2Frequency(float destination,
                                         float intensity) noexcept
{
   m_modifierFrequency += (destination - m_frequency) * intensity;
}

inline void LFO::applyModifier2MultiplierExp(float destination,
                                             float intensity) noexcept
{
   m_modifierMultiplierExp +=
       (destination * MAX_MULTIPLIER_EXP - m_multiplierExp) * intensity;
}

inline Waveform LFO::waveform() const noexcept
{
   if (mpark::holds_alternative<Sine>(m_waveform))
      return Waveform::Sine;
   else if (mpark::holds_alternative<Square>(m_waveform))
      return Waveform::Square;
   else if (mpark::holds_alternative<Triangle>(m_waveform))
      return Waveform::Triangle;
   else if (mpark::holds_alternative<Saw>(m_waveform))
      return Waveform::Saw;
   else if (mpark::holds_alternative<Random>(m_waveform))
      return Waveform::Random;
   return Waveform::Sine;   // should not happen
}

inline float LFO::waveformAsFloat() const noexcept
{
   return static_cast<float>(waveform()) /
          mpark::variant_size_v<decltype(m_waveform)>;
}

inline float LFO::amplitude() const noexcept { return m_amplitude; }

inline float LFO::frequency() const noexcept { return m_frequency; }

inline uint32_t LFO::multiplierExp() const noexcept { return m_multiplierExp; }

inline float LFO::multiplierExpAsFloat() const noexcept
{
   return float(m_multiplierExp) / MAX_MULTIPLIER_EXP;
}

inline void LFO::reset() noexcept
{
   setAmplitude(DefaultAmplitude);
   setFrequency(DefaultFrequency);
   setMultiplierExp(DefaultMultiplierExp);
   m_waveform.emplace<Sine>();
}

inline bool LFO::getAndResetJustGotDisabled() noexcept
{
   bool ret          = m_justGotDisabled;
   m_justGotDisabled = false;
   return ret;
}

template <typename... Ts>
[[nodiscard]] mpark::variant<Ts...> expand_type(std::size_t i)
{
   assert(i < sizeof...(Ts));
   static constexpr mpark::variant<Ts...> table[] = {Ts{}...};
   return table[i];
}

inline LFO::WaveformVariant LFO::modifiedWaveform() const noexcept
{
   const int varIndex =
       util::clip(0, int(m_waveform.index() + m_modifierWaveform),
                  int(mpark::variant_size_v<decltype(m_waveform)> - 1));
   return expand_type<Sine, Square, Triangle, Saw, Random>(varIndex);
}

inline float LFO::modifiedAmplitude() const noexcept
{
   return util::clip(0.0f, m_amplitude + m_modifierAmplitude, 1.0f);
}

inline float LFO::modifiedFrequency() const noexcept
{
   return util::clip(0.0f, m_frequency + m_modifierFrequency, 1.0f);
}

inline uint32_t LFO::modifiedMultiplierExp() const noexcept
{
   return util::clip(0, int(m_multiplierExp + m_modifierMultiplierExp),
                     int(MAX_MULTIPLIER_EXP));
}

inline float LFO::Sine::operator()(float t) const noexcept
{
   return sin(2 * M_PI * t);
}

inline float LFO::Square::operator()(float t) const noexcept
{
   return (t <= switchAt ? (-1.0) : (1.0));
}

inline float LFO::Triangle::operator()(float t) const noexcept
{
   constexpr float b = 4.0;
   if (t < 0.25)
      return b * t;
   else if (0.25 <= t && t < 0.75)
      return -b * (t - 0.5);
   else
      return b * (t - 1.0);
}

inline float LFO::Saw::operator()(float t) const noexcept
{
   constexpr float b = 2.0;
   return b * (t - 0.5);
}

inline float LFO::Random::operator()(float t) noexcept
{
   if (t == 0)
   {
      std::random_device r;
      std::default_random_engine e1(r());
      std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);
      lastValue = uniform_dist(e1);
   }
   return lastValue;
}

}   // namespace base::musicDevice::sound::lfo