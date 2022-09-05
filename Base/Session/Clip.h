#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <map>
#include "NoteEvent.h"
#include "ParameterEvent.h"

namespace base::session
{

struct Clip
{
    std::string name;
    Beat startTime;
    Beat sequenceLength;
    std::pmr::map<NoteEvent>      m_noteEvents;
    std::pmr::map<ParameterEvent> m_parameterEvents;
};

}   // namespace base::session

#endif