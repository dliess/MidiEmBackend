#ifndef INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL

namespace base::instruments::loader
{
template <typename T> void KitInstrument::addVoice(int padIdx, T&& voice)
{
   m_voices[padIdx] = std::move(voice);
};

inline std::string KitInstrument::name() const noexcept { return m_name; }

inline void KitInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}

inline std::vector<KitVoice>& KitInstrument::voices() noexcept { return m_voices; }

inline const std::vector<KitVoice>& KitInstrument::voices() const noexcept
{
   return m_voices;
}

template <typename Cb> void KitInstrument::forEachComponent(Cb&& cb)
{
   for (auto& voice : m_voices)
   {
      for (auto& component : voice.components) { cb(component); }
   }
}

template <typename Cb> void KitInstrument::forEachComponent(Cb&& cb) const
{
   for (const auto& voice : m_voices)
   {
      for (const auto& component : voice.components) { cb(component); }
   }
}

template <typename Cb> void KitInstrument::forEachComponentExt(Cb&& cb)
{
   for (size_t voiceIdx = 0; voiceIdx < m_voices.size(); ++voiceIdx)
   {
      for (size_t componentIdx = 0;
           componentIdx < m_voices[voiceIdx].components.size(); ++componentIdx)
      {
         cb(m_voices[voiceIdx].components[componentIdx], voiceIdx, componentIdx);
      }
   }
}

template <typename Cb> void KitInstrument::forEachComponentExt(Cb&& cb) const
{
   for (size_t voiceIdx = 0; voiceIdx < m_voices.size(); ++voiceIdx)
   {
      for (size_t componentIdx = 0;
           componentIdx < m_voices[voiceIdx].components.size(); ++componentIdx)
      {
         cb(m_voices[voiceIdx].components[componentIdx], voiceIdx, componentIdx);
      }
   }
}

inline void KitInstrument::updateParameterUI() const
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components)
      {
         component.updateParameterUI();
      }
   }
}

inline void KitInstrument::withComponent(
    int voiceIdx, int componentIdx,
    util::function_ref<void(const Component&)> cb) const
{
   cb(m_voices.at(voiceIdx).components.at(componentIdx));
}

inline bool isSameInstrument(const KitInstrument& lhs, const KitInstrument& rhs)
{
   return lhs.m_name == rhs.m_name && lhs.m_voices == rhs.m_voices;
}

}   // namespace base::instruments

#endif
