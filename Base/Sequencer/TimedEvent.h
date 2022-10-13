#ifndef BASE_SEQUENCER_TIMED_EVENT_H
#define BASE_SEQUENCER_TIMED_EVENT_H

#include <cstdint>
namespace base::sequencer
{

using NoteId = uint32_t;
using Beat = double;

struct TimedEvent 
{
    NoteId id{0};
    Beat beatstamp{0.0};
};

}   // namespace base::sequencer

#endif