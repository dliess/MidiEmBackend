#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <set>
#include "NoteEvent.h"
#include "ParameterEvent.h"
#include "TimedEventContainer.h"

namespace base::session
{

struct Clip
{
    std::string name;
    sequencer::Beat startTime;
    sequencer::Beat prevClipBeat;
    sequencer::Beat sequenceLength;
    TimedEventContainer<std::set<sequencer::NoteEvent, sequencer::NoteEventCompare>>           noteEvents;
    TimedEventContainer<std::set<sequencer::ParameterEvent, sequencer::ParameterEventCompare>> parameterEvents;

    template<typename Container, typename Cb>
    void forNoteEvents(const Container& container, Cb&& cb);
};

}   // namespace base::session

#endif