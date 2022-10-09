#include "Tracks.h"
#include "BeatTick.h"

using namespace base::session;

Tracks::Tracks() :
   m_memoryPool("SessionTracks"),
   tracks(&m_memoryPool.pool()) {}

void Tracks::update()
{
   for (auto& track : tracks)
   {
      track.update();
   }
}

void Tracks::pushBackTrack(std::string_view name)
{
   tracks.emplace_back(name);
}

void Tracks::addTrack(std::string_view name, int position)
{
   const auto it = std::next(tracks.begin(), position);
   tracks.emplace(it, name);
}

void Tracks::duplicateTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this](auto it){
      tracks.insert(std::next(it), it->duplicate(&m_memoryPool.pool()));
   });
}

void Tracks::removeTrack(util::Identifiable::UUIDView uuid)
{
   withTrackIter(uuid, [this](auto it){
      tracks.erase(it);
   });
}

void Tracks::renameTrack(util::Identifiable::UUIDView uuid, std::string_view name)
{
   withTrackIter(uuid, [this, name](auto it){
      it->name = name;
   });
}

void Tracks::moveTrack(util::Identifiable::UUIDView uuid, int afterPosition)
{
   withTrackIter(uuid, [this, afterPosition](auto it){
      Track track(std::move(*it));
      tracks.erase(it);
      tracks.insert(std::next(tracks.begin(), afterPosition), std::move(track));
   });
}

