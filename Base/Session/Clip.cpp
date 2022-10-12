#include "Clip.h"
#include <math.h> /* fmod */
#include "BeatTick.h"

using namespace base;

session::Clip::Clip(const allocator_type& alloc) noexcept :
    name(alloc), noteEvents(alloc), parameterEvents(alloc)
{
}

session::Clip::Clip(Clip&& other, const allocator_type& alloc) noexcept :
    name(std::move(other.name), alloc),
    noteEvents(std::move(other.noteEvents), alloc),
    parameterEvents(std::move(other.parameterEvents), alloc)
{
}

session::Clip::Clip(const Clip& other, const allocator_type& alloc) :
    name(other.name, alloc),
    noteEvents(other.noteEvents, alloc),
    parameterEvents(other.parameterEvents, alloc)
{
}

void session::Clip::update(instruments::Instrument* instrument)
{
   sequencer::Beat clipBeat =
       tempo::BeatTick::instance().getBeat() - startTime;
   clipBeat = std::fmod(clipBeat, sequenceLength);
   noteEvents.forNoteEvents(
       prevClipBeat, clipBeat,
       [this, instrument](const sequencer::NoteEvent& noteEvent) {
          if (instrument)
          {
             switch (noteEvent.type)
             {
                case sequencer::NoteEvent::Event::On:
                {
                   instrument->noteOn(noteEvent.note, noteEvent.velocity);
                   break;
                }
                case sequencer::NoteEvent::Event::Off:
                {
                   instrument->noteOff(noteEvent.note, noteEvent.velocity);
                   break;
                }
             }
          }
       });
   prevClipBeat = clipBeat;
}