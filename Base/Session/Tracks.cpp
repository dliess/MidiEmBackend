#include "Tracks.h"
#include "BeatTick.h"

using namespace base::session;

Tracks::Tracks() :
   m_memoryPool("SessionTracks"),
   m_tracks(&m_memoryPool.pool()) {}

void Tracks::start()
{
   if(!m_started)
   {
      m_started = true;
      for (auto& track : m_tracks)
      {
         track.resetActiveClip();
      }
   }
}

void Tracks::stop()
{
   m_started = false;
   for (auto& track : m_tracks)
   {
      track.stop();
   }
}

void Tracks::update()
{
   if(!m_started) return;
   for (auto& track : m_tracks)
   {
      track.update();
   }
}

void Tracks::pushBackTrack(std::string_view name)
{
   auto& track = m_tracks.emplace_back(name);
   emitTrackAdded(track.idView(), name, m_tracks.size());
   track.onNameChanged([this, &track](std::string_view){
      emitTrackNameChanged(track.idView(), track.name());
   });
   // TODO ...
}

void Tracks::addTrack(std::string_view name, int position)
{
   const auto it = std::next(m_tracks.begin(), position);
   const auto insertIt = m_tracks.emplace(it, name);
   emitTrackAdded(insertIt->idView(), name, position);
}

void Tracks::duplicateTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this, &uuid](auto it){
      m_tracks.insert(std::next(it), it->duplicate(&m_memoryPool.pool()));
      emitTrackDuplicated(uuid);
   });
}

void Tracks::removeTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this, &uuid](auto it){
      m_tracks.erase(it);
      emitTrackRemoved(uuid);
   });
}

void Tracks::renameTrack(util::Identifiable::UUIDView uuid, std::string_view name)
{
   withTrackIter(uuid, [this, name](auto it){
      it->setName(name);
   });
}

void Tracks::moveTrack(util::Identifiable::UUIDView uuid, int afterPosition)
{
   withTrackIter(uuid, [this, afterPosition](auto it){
      Track track(std::move(*it));
      m_tracks.erase(it);
      m_tracks.insert(std::next(m_tracks.begin(), afterPosition), std::move(track));
   });
}

void Tracks::startClipRow(int row)
{
   for(auto& track : m_tracks) {
      track.startClip(row);
   }
}