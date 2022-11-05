#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <set>
#include <vector>
#include <list>
#include <map>
#include "NoteEvent.h"
#include "ParameterEvent.h"
#include "TimedEventContainer.h"
#include <memory_resource>
#include <cstddef> // std::byte
#include "Instrument.h"
#include "CallbackSignal.h"

namespace base::session
{

class Clip
{
public:
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    explicit Clip(const allocator_type& alloc) noexcept;
    Clip(const Clip& other, const allocator_type& alloc);
    Clip(Clip&& other, const allocator_type& alloc) noexcept;
    void update(instruments::Instrument* instrument);
    inline void setName(std::string_view nameV);
    inline void addNote(sequencer::Beat beat, sequencer::Beat length, int note, float velocity);
    inline void setNoteVelocity(sequencer::NoteId noteId, float velocity);
    inline void removeNote(sequencer::NoteId noteId);
    inline void removeAllNotes();
    void reset();
    void stop(instruments::Instrument* instrument);

    CB_SIGNAL_SINGLE_SUBSCRIBER(NameChanged, std::string_view);
    CB_SIGNAL_SINGLE_SUBSCRIBER(NoteAdded, sequencer::NoteId, sequencer::Beat, sequencer::Beat, int, float);
    CB_SIGNAL_SINGLE_SUBSCRIBER(NoteVelocityChanged, sequencer::NoteId, float);
    CB_SIGNAL_SINGLE_SUBSCRIBER(NoteRemoved, sequencer::NoteId);
    CB_SIGNAL_SINGLE_SUBSCRIBER(AllNotesRemoved);

private:
    std::pmr::string name;
    sequencer::Beat m_startBeat;
    sequencer::Beat m_prevClipBeat;
    sequencer::Beat m_sequenceLength;
    using NoteContainer = TimedEventContainer<std::pmr::map<sequencer::Beat, sequencer::NoteEvent>>;
    NoteContainer m_noteEvents;
    TimedEventContainer<std::pmr::set<sequencer::ParameterEvent, sequencer::ParameterEventCompare>> m_parameterEvents;
    std::pmr::list<const sequencer::NoteEvent*> m_activeNotes;
    sequencer::NoteId m_lastId{0};
};

}   // namespace base::session

#include "Clip.inl"

#endif