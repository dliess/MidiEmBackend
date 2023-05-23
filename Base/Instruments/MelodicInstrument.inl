#ifndef MELODIC_INSTRUMENT_INL
#define MELODIC_INSTRUMENT_INL

#include "MelodicInstrument.h"

namespace base::instruments
{
inline void MelodicInstrument::RtData::incrementVoiceIndex(int maxSize) noexcept
{
   ++m_currentVoiceIndex;
   if (m_currentVoiceIndex == maxSize)
   {
      m_currentVoiceIndex = 0;
   }
}

inline int MelodicInstrument::RtData::currentVoiceIndex() const noexcept
{
   return m_currentVoiceIndex;
}

inline void MelodicInstrument::updateParameterUI() const
{
   if(m_voices.size())
   {
      for(const auto& compPart : m_voices.at(0).components)
      {
         compPart.updateParameterUI();
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachComponent(Cb&& cb)
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components) { cb(component); }
   }
}

inline bool isSameInstrument(const MelodicInstrument& lhs,
                               const MelodicInstrument& rhs)
{
   return lhs.m_name == rhs.m_name && lhs.m_voices == rhs.m_voices;
}


}   // namespace base::instruments

#endif   // MELODIC_INSTRUMENT_INL