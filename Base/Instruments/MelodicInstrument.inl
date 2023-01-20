#ifndef MELODIC_INSTRUMENT_INL
#define MELODIC_INSTRUMENT_INL

#include "MelodicInstrument.h"

namespace base::instruments
{

inline void MelodicInstrument::RtData::incrementVoiceIndex() noexcept
{
   ++currentVoiceIndex;
   if(currentVoiceIndex == m_voices.size())
   {
      currentVoiceIndex = 0;
   }
}

inline int MelodicInstrument::RtData::currentVoiceIndex() const noexcept
{
   return m_currentVoiceIndex;
}

} // namespace base::instruments

#endif // MELODIC_INSTRUMENT_INL