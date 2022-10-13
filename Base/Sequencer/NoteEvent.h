#ifndef BASE_SEQUENCER_NOTE_EVENT_H
#define BASE_SEQUENCER_NOTE_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct NoteEvent : public TimedEvent
{
    int note{0};
    float velocity{0.0};
    Beat duration{0};
};

struct NoteEventCompare
{
    bool operator()(const NoteEvent& lhs, const NoteEvent& rhs) const
    {
        return lhs.beatstamp < rhs.beatstamp;
    }
};


}   // namespace base::sequencer

#endif