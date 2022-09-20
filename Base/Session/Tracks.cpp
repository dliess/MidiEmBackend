#include "Tracks.h"

#include <math.h> /* fmod */

#include "BeatTick.h"

using namespace base::session;

void Tracks::update()
{
   for (auto& track : tracks)
   {
      if (track.activeClip)
      {
         auto& clip = track.clips[track.activeClip.value()];
         sequencer::Beat clipBeat =
             tempo::BeatTick::instance().getBeat() - clip.startTime;
         clipBeat = std::fmod(clipBeat, clip.sequenceLength);
         clip.noteEvents.forNoteEvents(
             clip.prevClipBeat, clipBeat,
             [&track](const sequencer::NoteEvent& noteEvent) {
                if (track.instrument)
                {
                   switch (noteEvent.type)
                   {
                      case sequencer::NoteEvent::Event::On:
                      {
                         track.instrument->noteOn(noteEvent.note,
                                                  noteEvent.velocity);
                         break;
                      }
                      case sequencer::NoteEvent::Event::Off:
                      {
                         track.instrument->noteOff(noteEvent.note,
                                                   noteEvent.velocity);
                         break;
                      }
                   }
                }
             });
         clip.prevClipBeat = clipBeat;
      }
   }
}

void Tracks::pushBackTrack(std::string name)
{
   tracks.emplace_back(std::move(name));
}

void Tracks::addTrack(std::string name, int position)
{
   //tracks.insert();
}
