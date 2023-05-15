#ifndef BASE_SEQUENCER_PARAMETER_EVENT_H
#define BASE_SEQUENCER_PARAMETER_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct ParameterEvent
{
    NoteId id{0};
    Beat beatstamp{0.0};
    int parameterIdx{0};
    float value{0.0};
};

struct ParameterEventCompare
{
    bool operator()(const ParameterEvent& lhs, const ParameterEvent& rhs) const
    {
        return lhs.beatstamp < rhs.beatstamp;
    }
};

}   // namespace base::sequencer

#endif