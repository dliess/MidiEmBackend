#ifndef BASE_SESSION_CLIP_INL
#define BASE_SESSION_CLIP_INL

#include <math.h> /* fmod */

namespace base::session
{
inline void Clip::setName(std::string_view nameV) { name = nameV; }

inline void Clip::addNote(sequencer::Beat beat, sequencer::Beat length,
                          int note, float velocity)
{
   const auto beatToInsert = std::fmod(beat, sequenceLength);
   noteEvents.insert(std::make_pair(
       beatToInsert,
       sequencer::NoteEvent{++m_lastId, beatToInsert, note, velocity, length}));
}

inline void Clip::setNoteVelocity(sequencer::NoteId noteId, float velocity)
{
   auto it = std::find_if(noteEvents.begin(), noteEvents.end(),
                          [noteId](const auto& pair) {
                             return pair.second.id == noteId;
                          });
   if (it != noteEvents.end())
   {
      it->second.velocity = velocity;
   }
}

inline void Clip::removeNote(sequencer::NoteId noteId)
{
  auto it = std::find_if(noteEvents.begin(), noteEvents.end(),
                          [noteId](const auto& pair) {
                             return pair.second.id == noteId;
                          });
   if (it != noteEvents.end())
   {
      noteEvents.erase(it);
   }
}

inline void Clip::removeAllNotes()
{
    noteEvents.clear();
}


}   // namespace base::session

#endif