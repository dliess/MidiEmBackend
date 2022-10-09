#include "Track.h"
#include "BeatTick.h"
#include <math.h> /* fmod */

using namespace base;

session::Track::Track(std::string_view name,
                      const allocator_type& alloc) noexcept :
    name(name, alloc), clips(alloc)
{
}

session::Track::Track(const Track& rhs, const allocator_type& alloc) :
    name(rhs.name, alloc), clips(rhs.clips, alloc)
{
}

session::Track::Track(Track&& rhs, const allocator_type& alloc) noexcept :
    name(std::move(rhs.name), alloc), clips(std::move(rhs.clips), alloc)
{
}

session::Track session::Track::duplicate(
    const allocator_type& alloc) const noexcept
{
   return Track(*this, alloc);
}

void session::Track::update()
{
   if (activeClip)
   {
      auto& clip = clips[activeClip.value()];
      sequencer::Beat clipBeat =
          tempo::BeatTick::instance().getBeat() - clip.startTime;
      clipBeat = std::fmod(clipBeat, clip.sequenceLength);
      clip.noteEvents.forNoteEvents(
          clip.prevClipBeat, clipBeat,
          [this](const sequencer::NoteEvent& noteEvent) {
             if (instrument)
             {
                switch (noteEvent.type)
                {
                   case sequencer::NoteEvent::Event::On:
                   {
                      instrument->noteOn(noteEvent.note,
                                               noteEvent.velocity);
                      break;
                   }
                   case sequencer::NoteEvent::Event::Off:
                   {
                      instrument->noteOff(noteEvent.note,
                                                noteEvent.velocity);
                      break;
                   }
                }
             }
          });
      clip.prevClipBeat = clipBeat;
   }
}

void session::Track::toggleMute() noexcept
{
   // TODO
}
