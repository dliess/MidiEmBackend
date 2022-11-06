#include "Tracks.h"

#include "BeatTick.h"

using namespace base::session;

Tracks::Tracks() : m_memoryPool("SessionTracks"), m_tracks(&m_memoryPool.pool())
{
}

void Tracks::start()
{
   if (!m_started)
   {
      m_started = true;
      for (auto& track : m_tracks) { track.resetActiveClip(); }
   }
}

void Tracks::stop()
{
   m_started = false;
   for (auto& track : m_tracks) { track.stop(); }
}

void Tracks::update()
{
   if (!m_started)
      return;
   for (auto& track : m_tracks) { track.update(); }
}

void Tracks::pushBackTrack(std::string_view name)
{
   auto& track = m_tracks.emplace_back(name);
   registerCbs(track);
   emitTrackAdded(track.idView(), name, m_tracks.size());
}

void Tracks::addTrack(std::string_view name, int position)
{
   const auto it       = std::next(m_tracks.begin(), position);
   const auto insertIt = m_tracks.emplace(it, name);
   registerCbs(*insertIt);
   emitTrackAdded(insertIt->idView(), name, position);
}

void Tracks::duplicateTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this, &uuid](auto it) {
      auto iter = m_tracks.insert(std::next(it), it->duplicate(&m_memoryPool.pool()));
      registerCbs(*iter);
      emitTrackDuplicated(uuid, iter->idView());
   });
}

void Tracks::removeTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this, &uuid](auto it) {
      m_tracks.erase(it);
      emitTrackRemoved(uuid);
   });
}

void Tracks::renameTrack(util::Identifiable::UUIDView uuid,
                         std::string_view name)
{
   withTrackIter(uuid, [this, name](auto it) { it->setName(name); });
}

void Tracks::moveTrack(util::Identifiable::UUIDView uuid, int afterPosition)
{
   withTrackIter(uuid, [this, afterPosition](auto it) {
      Track track(std::move(*it));
      m_tracks.erase(it);
      m_tracks.insert(std::next(m_tracks.begin(), afterPosition),
                      std::move(track));
   });
}

void Tracks::startClipRow(int row)
{
   for (auto& track : m_tracks) { track.startClip(row); }
}

void Tracks::registerCbs(Track& track)
{
   track.onNameChanged([this, &track](std::string_view) {
      emitTrackNameChanged(track.idView(), track.name());
   });
   track.onClipCreated(
       [this, &track](int row) { emitTrackClipCreated(track.idView(), row); });
   track.onClipDeleted(
       [this, &track](int row) { emitTrackClipDeleted(track.idView(), row); });
   track.onClipNameChanged([this, &track](int row, std::string_view name) {
      emitTrackClipNameChanged(track.idView(), row, name);
   });
   track.onClipNoteAdded(
       [this, &track](int row, sequencer::NoteId noteId, sequencer::Beat start,
                      sequencer::Beat length, int note, float velocity) {
          emitTrackClipNoteAdded(track.idView(), row, noteId, start, length,
                                 note, velocity);
       });
   track.onClipNoteVelocityChanged(
       [this, &track](int row, sequencer::NoteId noteId, float velocity) {
          emitTrackClipNoteVelocityChanged(track.idView(), row, noteId,
                                           velocity);
       });
   track.onClipNoteRemoved([this, &track](int row, sequencer::NoteId noteId) {
      emitTrackClipNoteRemoved(track.idView(), row, noteId);
   });
   track.onClipAllNotesRemoved([this, &track](int row) {
      emitTrackClipAllNotesRemoved(track.idView(), row);
   });
}