#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <set>
#include "NoteEvent.h"
#include "ParameterEvent.h"
#include "TimedEventContainer.h"
#include <memory_resource>
#include <cstddef> // std::byte
#include "Instrument.h"

namespace base::session
{

struct Clip
{
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    explicit Clip(const allocator_type& alloc) noexcept;
    Clip(const Clip& other, const allocator_type& alloc);
    Clip(Clip&& other, const allocator_type& alloc) noexcept;
    void update(instruments::Instrument* instrument);
    std::pmr::string name;
    sequencer::Beat startTime;
    sequencer::Beat prevClipBeat;
    sequencer::Beat sequenceLength;
    TimedEventContainer<std::pmr::set<sequencer::NoteEvent, sequencer::NoteEventCompare>>           noteEvents;
    TimedEventContainer<std::pmr::set<sequencer::ParameterEvent, sequencer::ParameterEventCompare>> parameterEvents;
};

}   // namespace base::session

#endif