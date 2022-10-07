#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"
#include "Identifiable.h"
#include <vector>
#include <string_view>
#include "stack_mempool.h"

namespace base::session
{

struct Tracks
{
private:
    util::StackMempool<32768> m_memoryPool;
public:
    Tracks();
    void update();
    void pushBackTrack(std::string_view name);
    void addTrack(std::string_view name, int position);
    void duplicateTrack(util::Identifiable::UUIDView uuid);
    std::pmr::vector<Track> tracks;
};

}   // namespace session

#endif