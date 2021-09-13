#include "LFO.h"

#include <cmath>
#include "BeatTick.h"
#include "Overload.h"

namespace base::musicDevice::sound
{

inline bool LFO::enabled() const noexcept
{
   return (m_amplitude != 0.0) && (m_frequency != 0.0);
}

inline float LFO::calculateValue() const noexcept 
{
   const auto jiffies = tempo::BeatTick::instance().getBeatJiffies();
   const float t = ((jiffies % tempo::BeatTick::PPQ) / float(tempo::BeatTick::PPQ)) * m_frequency;
   const auto fnVal = mpark::visit(util::overload{
      [t](auto && f){ return f(t); }
   }, m_waveform);
   return m_amplitude * fnVal;
};

inline bool LFO::setWaveform(Waveform waveform) noexcept
{
   switch(waveform)
   {
      case Waveform::Sine: m_waveform.emplace<Sine>(); return true;
      case Waveform::Square: m_waveform.emplace<Square>(); return true;
      case Waveform::Triangle: /*TODO*/ return true;
      case Waveform::Saw: /*TODO*/ return true;
      case Waveform::Random: /*TODO*/ return true;
   }
   return false;
}

inline bool LFO::setAmplitude(float amplitude) noexcept
{
   if(m_amplitude != amplitude && amplitude >= 0.0 && amplitude <= 1.0)
   {
      m_amplitude = amplitude;
      return true;
   }
   return false;
}

inline bool LFO::setFrequency(float frequency) noexcept
{
   if(m_frequency != frequency && frequency >= 0.0 && frequency <= 1.0)
   {
      m_frequency = frequency;
      return true;
   }
   return false;
}

inline LFO::Waveform LFO::waveform() const noexcept
{
   if(mpark::holds_alternative<Sine>(m_waveform))
      return Waveform::Sine;
   else if(mpark::holds_alternative<Square>(m_waveform))
      return Waveform::Square;
   else if(mpark::holds_alternative<Triangle>(m_waveform))
      return Waveform::Triangle;
   else if(mpark::holds_alternative<Saw>(m_waveform))
      return Waveform::Saw;
   else if(mpark::holds_alternative<Random>(m_waveform))
      return Waveform::Random;
   return Waveform::Sine; // should not happen
}

inline float LFO::amplitude() const noexcept
{
   return m_amplitude;
}

inline float LFO::frequency() const noexcept
{
   return m_frequency;
}

inline void LFO::reset() noexcept
{
   setAmplitude(0);
   setFrequency(0);
   m_waveform.emplace<Sine>();
}

inline float LFO::Sine::operator()(float t) const noexcept
{
   return sin(2 * M_PI * t);
}

inline float LFO::Square::operator()(float t) const noexcept
{
   const float ret = (t <= switchAt ? (-1.0) : (1.0));
   return inverted ? -1.0 * ret : ret;
}

inline float LFO::Triangle::operator()(float t) const noexcept
{
   return sin(2 * M_PI * t);
}

inline float LFO::Saw::operator()(float t) const noexcept
{
   return sin(2 * M_PI * t);
}

inline float LFO::Random::operator()(float t) const noexcept
{
   return sin(2 * M_PI * t);
}

} // namespace base::musicDevice::sound