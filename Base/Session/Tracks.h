#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"
#include <vector>
#include <string>
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
    void pushBackTrack(std::string name);
    void addTrack(std::string name, int position);
    std::pmr::vector<Track> tracks;
};

}   // namespace session

#endif