#ifndef MELODIC_INSTRUMENT_INL
#define MELODIC_INSTRUMENT_INL

#include "MelodicInstrument.h"

namespace base::instruments
{

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