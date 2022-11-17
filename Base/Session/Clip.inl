#ifndef BASE_SESSION_CLIP_INL
#define BASE_SESSION_CLIP_INL

#include <math.h> /* fmod */

namespace base::session
{
inline void Clip::setName(std::string_view nameV)
{
   m_name = nameV;
   emitNameChanged(m_name);
}

inline std::string_view Clip::name() const { return m_name; }

inline void Clip::addNote(sequencer::Beat beat, sequencer::Beat length,
                          int note, float velocity)
{
   const auto beatToInsert = std::fmod(beat, m_sequenceLength);
   auto [it, success]      = m_noteEvents.insert(std::make_pair(
       beatToInsert,
       sequencer::NoteEvent{++m_lastId, beatToInsert, note, velocity, length}));
   if (success)
   {
      emitNoteAdded(it->second.id, it->second.beatstamp, it->second.duration,
                    it->second.note, it->second.velocity);
   }
}

inline void Clip::setNoteVelocity(sequencer::NoteId noteId, float velocity)
{
   auto it = std::find_if(
       m_noteEvents.begin(), m_noteEvents.end(),
       [noteId](const auto& pair) { return pair.second.id == noteId; });
   if (it != m_noteEvents.end())
   {
      it->second.velocity = velocity;
      emitNoteVelocityChanged(noteId, velocity);
   }
}

inline void Clip::setNoteLength(sequencer::NoteId noteId,
                                sequencer::Beat length)
{
   auto it = std::find_if(
       m_noteEvents.begin(), m_noteEvents.end(),
       [noteId](const auto& pair) { return pair.second.id == noteId; });
   if (it != m_noteEvents.end())
   {
      it->second.duration = length;
      emitNoteLengthChanged(noteId, length);
   }
}

inline void Clip::setNoteStartBeat(sequencer::NoteId noteId,
                                   sequencer::Beat startBeat)
{
   auto it = std::find_if(
       m_noteEvents.begin(), m_noteEvents.end(),
       [noteId](const auto& pair) { return pair.second.id == noteId; });
   if (it != m_noteEvents.end())
   {
      it->second.beatstamp = startBeat;
      emitNoteStartBeatChanged(noteId, startBeat);
   }
}

inline void Clip::removeNote(sequencer::NoteId noteId)
{
   auto it = std::find_if(
       m_noteEvents.begin(), m_noteEvents.end(),
       [noteId](const auto& pair) { return pair.second.id == noteId; });
   if (it != m_noteEvents.end())
   {
      m_noteEvents.erase(it);
      emitNoteRemoved(noteId);
   }
}

inline void Clip::removeAllNotes()
{
   m_noteEvents.clear();
   emitAllNotesRemoved();
}

inline void Clip::setSequenceLength(sequencer::Beat seqLen)
{
   if (m_sequenceLength != seqLen)
   {
      m_sequenceLength = seqLen;
      emitSequenceLengthChanged(m_sequenceLength);
   }
}

}   // namespace base::session

#endif