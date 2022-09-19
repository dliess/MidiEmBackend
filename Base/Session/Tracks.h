#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"
#include <string>

namespace base::session
{

struct Tracks
{
    void update();
    void pushBackTrack(std::string name);
    void addTrack(std::string name, int position);
    std::vector<Track> tracks;
};

}   // namespace session

#endif