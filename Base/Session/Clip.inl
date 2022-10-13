#ifndef BASE_SESSION_CLIP_INL
#define BASE_SESSION_CLIP_INL

#include <math.h> /* fmod */

namespace base::session
{
inline void Clip::setName(std::string_view nameV) { name = nameV; }

inline void Clip::addNote(sequencer::Beat beat, sequencer::Beat length,
                          int note, float velocity)
{
    /*
   auto [itNoteOn, dummy1] = noteEvents.insert(sequencer::NoteEvent{std::fmod(beat, sequenceLength),
                                          sequencer::NoteEvent::Event::On, note,
                                          velocity});
   auto [itNoteOff, dummy2] = noteEvents.insert(
       sequencer::NoteEvent{std::fmod(beat + length, sequenceLength),
                            sequencer::NoteEvent::Event::Off, note, 1.0});
   itNoteOn->otherside = &(*itNoteOff);
   itNoteOff->otherside = &(*itNoteOfn);
   */
}

}   // namespace base::session

#endif