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
   calculateValueMods();
   const auto beat   = tempo::BeatTick::instance().getBeat();
   auto deltaBeat    = beat - m_beatAtWaveStart;
   const auto period = 1.0 / (m_actualFrequency * (1 << m_actualMultiplierExp));
   if (deltaBeat >= period)
   {
      m_beatAtWaveStart = beat;
      deltaBeat         = 0;
   }
   const auto t = deltaBeat / period;

   const auto fnVal = mpark::visit(
       util::overload{[t](auto&& f) { return f(t); }}, m_waveformVariant);
   return m_actualAmplitude * fnVal;
};

inline void LFO::calculateValueMods() noexcept
{
   if (m_actualWaveform != modifiedWaveform())
   {
      m_actualWaveform = modifiedWaveform();
      setWaveformVariant(m_actualWaveform);
      m_dirtyFlagsUi.waveform = true;
   }
   if (m_actualAmplitude != modifiedAmplitude())
   {
      m_actualAmplitude = modifiedAmplitude();
      if (m_actualAmplitude == 0.0)
      {
         m_justGotDisabled = true;
      }
      m_dirtyFlagsUi.amplitude = true;
   }
   if (m_actualFrequency != modifiedFrequency())
   {
      m_actualFrequency = modifiedFrequency();
      if (m_actualFrequency == 0.0)
      {
         m_justGotDisabled = true;
      }
      m_dirtyFlagsUi.frequency = true;
   }
   if (m_actualMultiplierExp != modifiedMultiplierExp())
   {
      m_actualMultiplierExp        = modifiedMultiplierExp();
      m_dirtyFlagsUi.multiplierExp = true;
   }
}

inline void LFO::setWaveform(Waveform waveform) noexcept
{
   if (m_waveform != waveform)
   {
      m_waveform              = waveform;
      m_dirtyFlagsUi.waveform = true;
   }
}

inline void LFO::setWaveformVariant(Waveform waveform) noexcept
{
   m_dirtyFlagsUi.waveform = true;
   switch (waveform)
   {
      case Waveform::Sine: m_waveformVariant.emplace<Sine>(); break;
      case Waveform::Square: m_waveformVariant.emplace<Square>(); break;
      case Waveform::Triangle: m_waveformVariant.emplace<Triangle>(); break;
      case Waveform::Saw: m_waveformVariant.emplace<Saw>(); break;
      case Waveform::Random: m_waveformVariant.emplace<Random>(); break;
   }
}

inline void LFO::setAmplitude(float amplitude) noexcept
{
   amplitude = util::clip(amplitude, 0.0f, 1.0f);
   if (m_amplitude != amplitude)
   {
      m_amplitude              = amplitude;
      m_dirtyFlagsUi.amplitude = true;
   }
}

inline void LFO::setFrequency(float frequency) noexcept
{
   frequency = util::clip(frequency, 0.0f, 1.0f);
   if (m_frequency != frequency && frequency >= 0.0 && frequency <= 1.0)
   {
      m_frequency              = frequency;
      m_dirtyFlagsUi.frequency = true;
   }
}

inline void LFO::setMultiplierExp(uint32_t multiplierExp) noexcept
{
   multiplierExp = util::clip(multiplierExp, uint32_t(1), MAX_MULTIPLIER_EXP);
   if (m_multiplierExp != multiplierExp)
   {
      m_multiplierExp = multiplierExp;
      m_dirtyFlagsUi.multiplierExp = true;
   }
}

inline void LFO::applyModifier2Waveform(float destination,
                                        float intensity) noexcept
{
   m_modifierWaveform +=
       (destination - static_cast<int>(m_waveform)) * intensity;
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
   m_modifierMultiplierExp += (destination - m_multiplierExp) * intensity;
}

inline Waveform LFO::waveform() const noexcept
{
   return m_actualWaveform;
}

inline float LFO::amplitude() const noexcept { return m_actualAmplitude; }

inline float LFO::frequency() const noexcept { return m_actualFrequency; }

inline uint32_t LFO::multiplierExp() const noexcept { return m_actualMultiplierExp; }

inline void LFO::reset() noexcept
{
   setAmplitude(DefaultAmplitude);
   setFrequency(DefaultFrequency);
   setMultiplierExp(DefaultMultiplierExp);
   setWaveform(DefaultWaveform);
}

inline bool LFO::getAndResetJustGotDisabled() noexcept
{
   bool ret          = m_justGotDisabled;
   m_justGotDisabled = false;
   return ret;
}

inline Waveform LFO::modifiedWaveform() const noexcept
{
   return static_cast<Waveform>(util::clip(
       float(static_cast<int>(m_waveform) + m_modifierWaveform), 0.0f,
       float(mpark::variant_size_v<decltype(m_waveformVariant)> - 1)));
}

inline float LFO::modifiedAmplitude() const noexcept
{
   return util::clip(m_amplitude + m_modifierAmplitude, 0.0f, 1.0f);
}

inline float LFO::modifiedFrequency() const noexcept
{
   return util::clip(m_frequency + m_modifierFrequency, 0.0f, 1.0f);
}

inline uint32_t LFO::modifiedMultiplierExp() const noexcept
{
   return util::clip(int(m_multiplierExp + m_modifierMultiplierExp), 0,
                     int(MAX_MULTIPLIER_EXP));
}

inline void LFO::clearModifiers() noexcept
{
   m_modifierWaveform      = 0;
   m_modifierAmplitude     = 0;
   m_modifierFrequency     = 0;
   m_modifierMultiplierExp = 0;
}

template <typename CB_amp, typename CB_freq, typename CB_waw, typename CB_mult>
void LFO::uiAsksForChanges(CB_amp&& cbAmp, CB_freq&& cbFreq, CB_waw&& cbWaw,
                           CB_mult&& cb_mult)
{
   if (m_dirtyFlagsUi.amplitude)
   {
      cbAmp(m_actualAmplitude);
      m_dirtyFlagsUi.amplitude = false;
   }
   if (m_dirtyFlagsUi.frequency)
   {
      cbFreq(m_actualFrequency);
      m_dirtyFlagsUi.frequency = false;
   }
   if (m_dirtyFlagsUi.waveform)
   {
      cbWaw(m_actualWaveform);
      m_dirtyFlagsUi.waveform = false;
   }
   if (m_dirtyFlagsUi.multiplierExp)
   {
      cb_mult(m_actualMultiplierExp);
      m_dirtyFlagsUi.multiplierExp = false;
   }
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