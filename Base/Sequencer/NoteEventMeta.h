#ifndef BASE_SEQUENCER_NOTE_EVENT_META_H
#define BASE_SEQUENCER_NOTE_EVENT_META_H

#include "JsonCast.h"
#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::sequencer::NoteEvent>()
{
   return members(
      member("noteId", &base::sequencer::NoteEvent::id),
      member("beat", &base::sequencer::NoteEvent::beatstamp),
      member("note", &base::sequencer::NoteEvent::note),
      member("velocity", &base::sequencer::NoteEvent::velocity),
      member("length", &base::sequencer::NoteEvent::length)
   );
}

} // namespace meta

#endif