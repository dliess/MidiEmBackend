#ifndef BASE_SEQUENCER_NOTE_EVENT_H
#define BASE_SEQUENCER_NOTE_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct NoteEvent : public TimedEvent
{
    int note{0};
    float velocity{0.0};
    Beat length{0};
};

}   // namespace base::sequencer

#include "NoteEventMeta.h"

#endif