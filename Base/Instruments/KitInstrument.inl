#ifndef INSTRUMENTS_KIT_INSTRUMENTS_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_INL

namespace base::instruments
{

template<typename T>
void KitInstrument::addSound(T&& kompositeSound) noexcept
{
   m_sounds.emplace_back(std::forward<T>(kompositeSound));
};

inline std::string KitInstrument::name() const noexcept { return m_name; }

inline void KitInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}

inline std::vector<CompositeSound>& KitInstrument::sounds() noexcept
{
   return m_sounds;
}

inline const std::vector<CompositeSound>& KitInstrument::sounds() const noexcept
{
   return m_sounds;
}

template <typename Cb>
void KitInstrument::forEachVoice(Cb&& cb)
{
   for(auto& sound : m_sounds)
   {
      for(auto& voice : sound.voices)
      {
         cb(voice);
      }
   }
}


} // namespace base::instruments

#endif