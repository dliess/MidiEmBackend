#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <set>
#include "NoteEvent.h"
#include "ParameterEvent.h"

namespace base::session
{

struct Clip
{
    std::string name;
    sequencer::Beat startTime;
    sequencer::Beat prevCheckTime;
    sequencer::Beat sequenceLength;
    std::set<sequencer::NoteEvent, sequencer::NoteEventCompare>           noteEvents;
    std::set<sequencer::ParameterEvent, sequencer::ParameterEventCompare> parameterEvents;
};

}   // namespace base::session

#endif