#ifndef BASE_SESSION_TRACK_INL
#define BASE_SESSION_TRACK_INL

#include "Instrument.h"

namespace base
{
inline void session::Track::setName(std::string_view name)
{
   m_name = name;
   emitNameChanged(m_name);
}

inline void session::Track::createClip(int row)
{
   if (!m_clips[row])
   {
      m_clips[row] = util::pmr::make_unique<Clip>(m_clips.get_allocator());
      registerCbs(row);
      emitClipCreated(row);
   }
}

inline void session::Track::createClip(int row, const session::Clip& clip)
{
   if (!m_clips[row])
   {
      m_clips[row] = util::pmr::make_unique(clip, m_clips.get_allocator());
      registerCbs(row);
      emitClipCreated(row);
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
      if (m_instrumentUUID)
      {
         m_instrumentsRef.withInstrumentRt(
             *m_instrumentUUID,
             [this, row](const instruments::Instrument& instrument) {
                m_clips[row]->stop(&instrument);
             });
      }
      m_clips[row].reset();
      emitClipDeleted(row);
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
         emitClipAboutToStart(row);
      }
   }
   else
   {
      m_toStartClipIdx = row;
      emitClipAboutToStart(row);
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

inline std::string_view session::Track::name() const { return m_name; }

inline void session::Track::setInstrumentUUID(
    util::Identifiable::UUIDView instrumentUUID)
{
   if (!m_instrumentUUID || (m_instrumentUUID != instrumentUUID))
   {
      if (m_instrumentUUID && m_activeClipIdx)
      {
         m_instrumentsRef.withInstrumentRt(
             *m_instrumentUUID,
             [this](const instruments::Instrument& instrument) {
                m_clips[*m_activeClipIdx]->stop(&instrument);
             });
      }
      m_instrumentUUID = util::deepCopy(instrumentUUID);
      emitInstrumentChanged(*m_instrumentUUID);
   }
}

inline std::optional<int> session::Track::startedClipIdx() const noexcept
{
   return m_activeClipIdx;
}

/*
inline void session::Track::noteOn(int note, float velocity) noexcept
{
   if (m_instrumentUUID)
   {
      m_instrument->noteOn(note, velocity);
   }
}

inline void session::Track::noteOff(int note, float velocity) noexcept
{
   if (m_instrument)
   {
      m_instrument->noteOff(note, velocity);
   }
}
*/
}   // namespace base

#endif