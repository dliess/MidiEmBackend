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
    sequencer::Beat sequenceLength;
    std::set<sequencer::NoteEvent>      m_noteEvents;
    std::set<sequencer::ParameterEvent> m_parameterEvents;
};

}   // namespace base::session

#endif