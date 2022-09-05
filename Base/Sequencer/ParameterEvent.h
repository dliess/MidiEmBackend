#ifndef BASE_SEQUENCER_PARAMETER_EVENT_H
#define BASE_SEQUENCER_PARAMETER_EVENT_H

#include "TimedEvent.h"

namespace base::sequencer
{

struct ParameterEvent : public TimedEvent
{
    int parameterIdx{0};
    float value{0.0};
};

}   // namespace base::sequencer

#endif