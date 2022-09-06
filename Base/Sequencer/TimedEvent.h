#ifndef BASE_SEQUENCER_TIMED_EVENT_H
#define BASE_SEQUENCER_TIMED_EVENT_H

namespace base::sequencer
{

using Beat = double;

struct TimedEvent 
{
    Beat beatstamp{0.0};
};

}   // namespace base::sequencer

#endif