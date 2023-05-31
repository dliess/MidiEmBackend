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
         if (component)
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
         if (component)
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
         if (m_voices[voiceIdx].components[componentIdx])
         {
            cb(m_voices[voiceIdx].components[componentIdx].value(), voiceIdx,
               componentIdx);
         }
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachLeadComponent(Cb&& cb)
{
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto component = getFirstComponent(componentIdx);
      if (component)
      {
         std::forward<Cb>(cb)(*component);
      }
   }
}

template <typename Cb>
void MelodicInstrument::forEachLeadComponent(Cb&& cb) const
{
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto component = getFirstComponent(componentIdx);
      if (component)
      {
         std::forward<Cb>(cb)(*component);
      }
   }
}

template <typename Cb> void MelodicInstrument::forEachLeadComponentExt(Cb&& cb)
{
   for (size_t componentIdx = 0;
        componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
        ++componentIdx)
   {
      auto component = getFirstComponent(componentIdx);
      if (component)
      {
         std::forward<Cb>(cb)(*component, componentIdx);
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

inline const Component* MelodicInstrument::getFirstComponent(
    size_t componentIdx) const
{
   for (const auto& voice : m_voices)
   {
      if (voice.components.containsComponent(componentIdx))
      {
         return &voice.components[componentIdx].value();
      }
   }
   return nullptr;
}

}   // namespace base::instruments

#endif   // MELODIC_INSTRUMENT_INL