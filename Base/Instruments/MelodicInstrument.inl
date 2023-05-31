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
   if (m_voices.size())
   {
      for (const auto& component : m_voices.at(0).components)
      {
         if(component)
         {
            component->updateParameterUI();
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachComponent(Cb&& cb)
{
   for (auto& voice : m_voices)
   {
      for (auto& component : voice.components)
      {
         if(component)
         {
            std::forward<Cb>(cb)(component.value());
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachComponentExt(Cb&& cb)
{
   for (size_t voiceIdx = 0; voiceIdx < m_voices.size(); ++voiceIdx)
   {
      for (size_t componentIdx = 0;
           componentIdx < m_voices[voiceIdx].components.size(); ++componentIdx)
      {
         if(m_voices[voiceIdx].components[componentIdx])
         {
            cb(m_voices[voiceIdx].components[componentIdx].value(), voiceIdx, componentIdx);
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachLeadComponent(Cb&& cb)
{
   if (m_voices.size() > 0)
   {
      for (auto& component : m_voices[LEAD_VOICE_IDX].components)
      {
         if(component)
         {
            std::forward<Cb>(cb)(component.value());
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachLeadComponent(Cb&& cb) const
{
   if (m_voices.size() > 0)
   {
      for (const auto& component : m_voices[LEAD_VOICE_IDX].components)
      {
         if(component)
         {
            std::forward<Cb>(cb)(component.value());
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachLeadComponentExt(Cb&& cb)
{
   if (m_voices.size() > 0)
   {
      for (size_t componentIdx = 0;
           componentIdx < m_voices[LEAD_VOICE_IDX].components.size();
           ++componentIdx)
      {
         if(m_voices[LEAD_VOICE_IDX].components[componentIdx])
         {
            std::forward<Cb>(cb)(m_voices[LEAD_VOICE_IDX].components[componentIdx].value(),
                                 componentIdx);
         }
      }
   }
}

inline bool isSameInstrument(const MelodicInstrument& lhs,
                             const MelodicInstrument& rhs)
{
   return lhs.m_name == rhs.m_name && lhs.m_voices == rhs.m_voices;
}

inline MelodicInstrument::VoiceContainer& MelodicInstrument::voices() noexcept
{
   return m_voices;
}

inline const MelodicInstrument::VoiceContainer& MelodicInstrument::voices()
    const noexcept
{
   return m_voices;
}

}   // namespace base::instruments

#endif   // MELODIC_INSTRUMENT_INL