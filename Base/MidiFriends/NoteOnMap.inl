
namespace base::midifriends
{

inline 
void NoteOnMap::setNoteOn(int voiceIdx, int note)
{
   m_map.at(voiceIdx).set(note);
}

inline
void NoteOnMap::setNoteOff(int voiceIdx, int note)
{
   m_map.at(voiceIdx).reset(note);
}

template<typename Cb> 
void NoteOnMap::forEachNoteOn(Cb&& cb)
{
   for (int x = 0; x < m_map.size(); ++x)
   {
      for (int y = 0; y < m_map[x].size(); ++y)
      {
         if (m_map[x][y])
         {
            cb(x, y);
         }
      }
   }
}

inline 
void NoteOnMap::clear() noexcept
{
   for (auto& bitset : m_map)
   {
      bitset.reset();
   }
}

} // namespace base::midifriends

