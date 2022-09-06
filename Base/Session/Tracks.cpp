#include "Tracks.h"
#include "BeatTick.h"
#include <math.h> /* fmod */

using namespace base::session;


void Tracks::update()
{
    for(auto& track : tracks)
    {
        if(track.activeClip)
        {
            auto& clip = track.clips[track.activeClip.value()];
            sequencer::Beat delta = tempo::BeatTick::instance().getBeat() - clip.startTime;
            delta = std::fmod(delta, clip.sequenceLength);
            const auto itStart = clip.noteEvents.upper_bound(sequencer::NoteEvent{delta});
            const auto itEnd = clip.noteEvents.lower_bound(sequencer::NoteEvent{clip.prevCheckTime});

        }
    }
}
