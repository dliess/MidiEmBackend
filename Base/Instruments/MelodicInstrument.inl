#ifndef MELODIC_INSTRUMENT_INL
#define MELODIC_INSTRUMENT_INL

#include "MelodicInstrument.h"

namespace base::instruments
{

template<typename T>
void MelodicInstrument::addVoice(T&& voice) noexcept
{
   m_voices.emplace_back(std::forward<T>(voice));
}

template <typename Cb>
void MelodicInstrument::forEachVoice(Cb&& cb)
{
   for(auto& voice : m_voices)
   {
      cb(voice);
   }
}

inline void MelodicInstrument::incrementVoiceIndex() noexcept
{
   if(m_voices.empty()) return;
   ++m_currentVoiceIndex;
   if(m_currentVoiceIndex == m_voices.size())
   {
      m_currentVoiceIndex = 0;
   }
}

} // namespace base::instruments

#endif // MELODIC_INSTRUMENT_INL