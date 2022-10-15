#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"
#include "Identifiable.h"
#include <vector>
#include <string_view>
#include "stack_mempool.h"

namespace base::session
{

class Tracks
{
public:
    Tracks();
    void update();
    void pushBackTrack(std::string_view name);
    void addTrack(std::string_view name, int position);
    void duplicateTrack(util::Identifiable::UUIDView uuid);
    void removeTrack(util::Identifiable::UUIDView uuid);
    void renameTrack(util::Identifiable::UUIDView uuid, std::string_view name);
    void moveTrack(util::Identifiable::UUIDView uuid, int afterPosition);
    template<typename CB>
    void withTrack(util::Identifiable::UUIDView uuid, CB&& cb);
    template<typename CB>
    void withClip(util::Identifiable::UUIDView uuid, int row, CB&& cb);
    void startClipRow(int row);
private:
    util::StackMempool<32768> m_memoryPool;
    template<typename CB>
    void withTrackIter(util::Identifiable::UUIDView uuid, CB&& cb);
    std::pmr::vector<Track> m_tracks;
};

}   // namespace session

#include "Tracks.inl"

#endif