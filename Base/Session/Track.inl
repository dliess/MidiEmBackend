#ifndef BASE_SESSION_TRACK_INL
#define BASE_SESSION_TRACK_INL

namespace base
{
inline void session::Track::setName(std::string_view name) { m_name = name; }

inline void session::Track::createClip(int row)
{
   if (!m_clips[row])
   {
      m_clips[row] = util::pmr::make_unique<Clip>(m_clips.get_allocator());
   }
}

inline void session::Track::createClip(int row, const session::Clip& clip)
{
   if (!m_clips[row])
   {
      m_clips[row] = util::pmr::make_unique(clip, m_clips.get_allocator());
   }
}

inline void session::Track::deleteClip(int row)
{
   if (m_toStartClipIdx && m_toStartClipIdx.value() == row)
      m_toStartClipIdx.reset();
   if (m_activeClipIdx && m_activeClipIdx.value() == row)
      m_activeClipIdx.reset();
   if (m_clips[row])
   {
      m_clips[row]->stop(m_instrument);
      m_clips[row].reset();
   }
}

inline void session::Track::startClip(int row)
{
   row = m_clips[row] ? row : StopperIdx;
   if (m_activeClipIdx)
   {
      if (m_activeClipIdx.value() != row)
      {
         m_toStartClipIdx = row;
      }
   }
   else
   {
      m_toStartClipIdx = row;
   }
}

inline void session::Track::stopClip() { m_toStartClipIdx = StopperIdx; }

inline session::Clip* session::Track::clip(int row) noexcept
{
   return m_clips[row].get();
}

inline const session::Clip* session::Track::clip(int row) const noexcept
{
   return m_clips[row].get();
}

}   // namespace base

#endif