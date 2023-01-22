#ifndef BASE_SESSION_CLIP_H
#define BASE_SESSION_CLIP_H

#include <cstddef>   // std::byte
#include <list>
#include <map>
#include <memory_resource>
#include <nlohmann/json.hpp>
#include <set>
#include <vector>

#include "CallbackSignal.h"
#include "Instrument.h"
#include "NoteEvent.h"
#include "ParameterEvent.h"
#include "TimedEventContainer.h"

namespace base::session
{
class Clip
{
public:
   using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
   explicit Clip(const allocator_type& alloc) noexcept;
   Clip(const Clip& other, const allocator_type& alloc);
   Clip(Clip&& other, const allocator_type& alloc) noexcept;
   void update(const instruments::Instrument* instrument);
   inline void setName(std::string_view nameV);
   inline std::string_view name() const;
   inline void addNote(sequencer::Beat beat, sequencer::Beat length, int note,
                       float velocity);
   inline void setNoteVelocity(sequencer::NoteId noteId, float velocity);
   inline void setNoteLength(sequencer::NoteId noteId, sequencer::Beat length);
   inline void setNoteStartBeat(sequencer::NoteId noteId,
                                sequencer::Beat startBeat);
   inline void removeNote(sequencer::NoteId noteId);
   inline void removeAllNotes();
   void reset();
   void stop(const instruments::Instrument* instrument);
   void setSequenceLength(sequencer::Beat);
   inline sequencer::Beat getSequenceLength() const noexcept;
   [[nodiscard]] inline sequencer::Beat getPrevClipBeat() const noexcept;

   CB_SIGNAL_SINGLE_SUBSCRIBER(NameChanged, std::string_view);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteAdded, sequencer::NoteId, sequencer::Beat,
                               sequencer::Beat, int, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteVelocityChanged, sequencer::NoteId, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteLengthChanged, sequencer::NoteId,
                               sequencer::Beat);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteStartBeatChanged, sequencer::NoteId,
                               sequencer::Beat);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteRemoved, sequencer::NoteId);
   CB_SIGNAL_SINGLE_SUBSCRIBER(AllNotesRemoved);
   CB_SIGNAL_SINGLE_SUBSCRIBER(SequenceLengthChanged, sequencer::Beat);

private:
   std::pmr::string m_name;
   sequencer::Beat m_startBeat;
   sequencer::Beat m_prevClipBeat;
   sequencer::Beat m_sequenceLength{4};
   using NoteContainer = TimedEventContainer<
       std::pmr::multimap<sequencer::Beat, sequencer::NoteEvent>>;
   NoteContainer m_noteEvents;
   TimedEventContainer<std::pmr::set<sequencer::ParameterEvent,
                                     sequencer::ParameterEventCompare>>
       m_parameterEvents;
   std::pmr::list<const sequencer::NoteEvent*> m_activeNotes;
   sequencer::NoteId m_lastId{0};

   friend void to_json(nlohmann::json& j, const Clip& p);
};

}   // namespace base::session

#include "Clip.inl"
#include "ClipMeta.h"

#endif