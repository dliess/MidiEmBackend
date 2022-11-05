#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"
#include "Identifiable.h"
#include <vector>
#include <string_view>
#include "stack_mempool.h"
#include "CallbackSignal.h"

namespace base::session
{

class Tracks
{
public:
    Tracks();
    void start();
    void stop();
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

    CB_SIGNAL(TrackAdded, util::Identifiable::UUIDView, std::string_view, int);
    CB_SIGNAL(TrackRemoved, util::Identifiable::UUIDView);
    CB_SIGNAL(TrackDuplicated, util::Identifiable::UUIDView);
    CB_SIGNAL(TrackMoved, util::Identifiable::UUIDView, int);
    CB_SIGNAL(TrackMuted, util::Identifiable::UUIDView, bool);

private:
    util::StackMempool<32768> m_memoryPool;
    template<typename CB>
    void withTrackIter(util::Identifiable::UUIDView uuid, CB&& cb);
    std::pmr::vector<Track> m_tracks;
    bool m_started{false};
};

}   // namespace session

#include "Tracks.inl"

#endif