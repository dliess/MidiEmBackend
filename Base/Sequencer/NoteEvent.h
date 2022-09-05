#ifndef BASE_SEQUENCER_NOTE_EVENT_H
#define BASE_SEQUENCER_NOTE_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct NoteEvent : public TimedEvent
{
    enum class Event {
        On,
        Off
    };
    Event type{Event::Off};
    int note{0};
};

}   // namespace base::sequencer

#endif