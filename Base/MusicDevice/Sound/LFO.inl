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

inline void LFO::setAmplitude(float amplitude) noexcept
{
   m_amplitude = amplitude;
}

inline void LFO::setFrequency(float frequency) noexcept
{
   m_frequency = frequency;
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

} // namespace base::musicDevice::sound