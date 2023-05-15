#ifndef BASE_SEQUENCER_NOTE_EVENT_H
#define BASE_SEQUENCER_NOTE_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct Note
{
    Beat beatstamp{0.0};
    int note{0};
    float velocity{0.0};
    Beat length{0};
};

struct NoteEvent
{
    NoteId id{0};
    Beat beatstamp{0.0};
    int note{0};
    float velocity{0.0};
    Beat length{0};
};

}   // namespace base::sequencer

#include "NoteEventMeta.h"

#endif