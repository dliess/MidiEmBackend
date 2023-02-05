#ifndef INSTRUMENTS_KIT_INSTRUMENTS_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_INL

namespace base::instruments
{
template <typename T>
void KitInstrument::addSound(int padIdx, T&& kompositeSound)
{
   m_compositeSounds[padIdx] = std::move(kompositeSound);
};

inline std::string KitInstrument::name() const noexcept { return m_name; }

inline void KitInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}

inline std::vector<CompositeSound>& KitInstrument::sounds() noexcept
{
   return m_compositeSounds;
}

inline const std::vector<CompositeSound>& KitInstrument::sounds() const noexcept
{
   return m_compositeSounds;
}

template <typename Cb> void KitInstrument::forEachVoice(Cb&& cb)
{
   for (auto& sound : m_compositeSounds)
   {
      for (auto& voice : sound.voices) { cb(voice); }
   }
}

inline void KitInstrument::withVoice(
    int soundIdx, int componentIdx,
    util::function_ref<void(const Voice&)> cb) const
{
   cb(m_compositeSounds.at(soundIdx).voices.at(componentIdx));
}

}   // namespace base::instruments

#endif