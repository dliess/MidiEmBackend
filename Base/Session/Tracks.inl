#ifndef BASE_SESSION_TRACKS_INL
#define BASE_SESSION_TRACKS_INL

namespace base::session
{

template<typename CB>
void Tracks::withTrackIter(util::Identifiable::UUIDView uuid, CB&& cb)
{
   auto it = std::find_if(tracks.begin(), tracks.end(), [&uuid](const Track& track){
      return track.idView() == uuid;
   });
   if(it == tracks.end())
   {
      spdlog::error("Could not find track with UUID {}", util::uuid2Str(uuid));
      return;
   }
   std::forward<CB>(cb)(it);
}

template<typename CB>
void Tracks::withTrack(util::Identifiable::UUIDView uuid, CB&& cb)
{
   withTrackIter(uuid, [&cb](auto it){
       cb(*it);
   });
}

}   // namespace base::session
#endif