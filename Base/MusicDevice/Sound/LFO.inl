#include <cmath>
#include <magic_enum.hpp>
#include <random>

#include "BeatTick.h"
#include "FloatEqual.h"
#include "LFO.h"
#include "Overload.h"
#include "clip.h"

namespace base::musicDevice::sound::lfo
{
inline bool LFO::enabled() const noexcept
{
   return (!util::floatEqual(m_actualAmplitude, 0.5f)) &&
          (m_actualFrequency != 0.0);
}

inline float LFO::calculateValue() noexcept
{
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
   return (m_actualAmplitude - 0.5f) * 2.0f * fnVal;
};

inline void LFO::calculateValueMods() noexcept
{
   if (!dirty())
      return;
   const Waveform mWaveform      = modifiedWaveform();
   const float mAmplitude        = modifiedAmplitude();
   const float mFrequency        = modifiedFrequency();
   const uint32_t mMultiplierExp = modifiedMultiplierExp();
   clearModifiers();

   if (m_actualWaveform != mWaveform)
   {
      m_actualWaveform = mWaveform;
      setWaveformVariant(m_actualWaveform);
      m_dirtyFlagsUi |= DirtyFlags::Waveform;
   }
   if (m_actualAmplitude != mAmplitude)
   {
      m_actualAmplitude = mAmplitude;
      if (util::floatEqual(m_actualAmplitude, 0.5f))
      {
         m_justGotDisabled = true;
      }
      m_dirtyFlagsUi |= DirtyFlags::Amplitude;
   }
   if (m_actualFrequency != mFrequency)
   {
      m_actualFrequency = mFrequency;
      if (m_actualFrequency == 0.0)
      {
         m_justGotDisabled = true;
      }
      m_dirtyFlagsUi |= DirtyFlags::Frequency;
   }
   if (m_actualMultiplierExp != mMultiplierExp)
   {
      m_actualMultiplierExp = mMultiplierExp;
      m_dirtyFlagsUi |= DirtyFlags::MultiplierExp;
   }
}

inline void LFO::setWaveform(Waveform waveform) noexcept
{
   if (m_waveform != waveform)
   {
      m_waveform = waveform;
      m_dirtyFlagsUi |= DirtyFlags::Waveform;
   }
}

inline void LFO::setWaveformVariant(Waveform waveform) noexcept
{
   m_dirtyFlagsUi |= DirtyFlags::Waveform;
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
      m_amplitude = amplitude;
      m_dirtyFlagsUi |= DirtyFlags::Amplitude;
   }
}

inline void LFO::setFrequency(float frequency) noexcept
{
   frequency = util::clip(frequency, 0.0f, 1.0f);
   if (m_frequency != frequency && frequency >= 0.0 && frequency <= 1.0)
   {
      m_frequency = frequency;
      m_dirtyFlagsUi |= DirtyFlags::Frequency;
   }
}

inline void LFO::setMultiplierExp(uint32_t multiplierExp) noexcept
{
   multiplierExp = util::clip(int(multiplierExp), 0, int(MAX_MULTIPLIER_EXP));
   if (m_multiplierExp != multiplierExp)
   {
      m_multiplierExp = multiplierExp;
      m_dirtyFlagsUi |= DirtyFlags::MultiplierExp;
   }
}

inline void LFO::applyModifier2Waveform(float destination,
                                        float intensity) noexcept
{
   m_waveformModifier.addAbsoluteModifier(destination, intensity);
}

inline void LFO::applyModifier2Amplitude(float destination,
                                         float intensity) noexcept
{
   m_amplitudeModifier.addAbsoluteModifier(destination, intensity);
}

inline void LFO::applyModifier2Frequency(float destination,
                                         float intensity) noexcept
{
   m_frequencyModifier.addAbsoluteModifier(destination, intensity);
}

inline void LFO::applyModifier2MultiplierExp(float destination,
                                             float intensity) noexcept
{
   m_multiplierExpModifier.addAbsoluteModifier(destination, intensity);
}

inline Waveform LFO::waveform() const noexcept { return m_actualWaveform; }

inline float LFO::amplitude() const noexcept { return m_actualAmplitude; }

inline float LFO::frequency() const noexcept { return m_actualFrequency; }

inline uint32_t LFO::multiplierExp() const noexcept
{
   return m_actualMultiplierExp;
}

inline bool LFO::dirty() const noexcept
{
   return m_dirtyFlagsUi != DirtyFlags::Empty;
}

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
   return Waveform(util::clip(
       static_cast<int>(calculateModifiedValue(m_waveform, m_waveformModifier)),
       0, int(magic_enum::enum_count<Waveform>() - 1)));
}

inline float LFO::modifiedAmplitude() const noexcept
{
   return util::clip(calculateModifiedValue(m_amplitude, m_amplitudeModifier),
                     0.0f, 1.0f);
}

inline float LFO::modifiedFrequency() const noexcept
{
   return util::clip(calculateModifiedValue(m_frequency, m_frequencyModifier),
                     0.0f, 1.0f);
}

inline uint32_t LFO::modifiedMultiplierExp() const noexcept
{
   return util::clip(
       calculateModifiedValue(m_multiplierExp, m_multiplierExpModifier), 0U,
       MAX_MULTIPLIER_EXP);
}

inline void LFO::clearModifiers() noexcept
{
   m_waveformModifier.clear();
   m_amplitudeModifier.clear();
   m_frequencyModifier.clear();
   m_multiplierExpModifier.clear();
}

template <typename CB_amp, typename CB_freq, typename CB_waw, typename CB_mult>
void LFO::uiAsksForChanges(CB_amp&& cbAmp, CB_freq&& cbFreq, CB_waw&& cbWaw,
                           CB_mult&& cb_mult)
{
   if (m_dirtyFlagsUi & DirtyFlags::Amplitude)
   {
      cbAmp(m_actualAmplitude);
   }
   if (m_dirtyFlagsUi & DirtyFlags::Frequency)
   {
      cbFreq(m_actualFrequency);
   }
   if (m_dirtyFlagsUi & DirtyFlags::Waveform)
   {
      cbWaw(m_actualWaveform);
   }
   if (m_dirtyFlagsUi & DirtyFlags::MultiplierExp)
   {
      cb_mult(m_actualMultiplierExp);
   }
   m_dirtyFlagsUi = DirtyFlags::Empty;
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