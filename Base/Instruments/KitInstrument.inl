#ifndef INSTRUMENTS_KIT_INSTRUMENTS_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_INL

namespace base::instruments
{
template <typename T>
void KitInstrument::addVoice(int padIdx, T&& voice)
{
   m_voices[padIdx] = std::move(voice);
};

inline std::string KitInstrument::name() const noexcept { return m_name; }

inline void KitInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}

inline std::vector<Voice>& KitInstrument::voices() noexcept
{
   return m_voices;
}

inline const std::vector<Voice>& KitInstrument::voices() const noexcept
{
   return m_voices;
}

template <typename Cb> void KitInstrument::forEachComponent(Cb&& cb)
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components) { cb(component); }
   }
}

inline void KitInstrument::updateParameterUI() const
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components) { component.updateParameterUI(); }
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
   return lhs.m_name == rhs.m_name &&
          lhs.m_voices == rhs.m_voices;
}

}   // namespace base::instruments

#endif