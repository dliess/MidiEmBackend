#include "Tracks.h"
#include "BeatTick.h"

using namespace base::session;

Tracks::Tracks() :
   m_memoryPool("SessionTracks"),
   m_tracks(&m_memoryPool.pool()) {}

void Tracks::update()
{
   for (auto& track : m_tracks)
   {
      track.update();
   }
}

void Tracks::pushBackTrack(std::string_view name)
{
   m_tracks.emplace_back(name);
}

void Tracks::addTrack(std::string_view name, int position)
{
   const auto it = std::next(m_tracks.begin(), position);
   m_tracks.emplace(it, name);
}

void Tracks::duplicateTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this](auto it){
      m_tracks.insert(std::next(it), it->duplicate(&m_memoryPool.pool()));
   });
}

void Tracks::removeTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this](auto it){
      m_tracks.erase(it);
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