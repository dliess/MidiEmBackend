#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include "Track.h"

namespace base::session
{

struct Tracks
{
    void update();
    std::vector<Track> tracks;
};

}   // namespace session

#endif