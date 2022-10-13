#include "Clip.h"

#include <math.h> /* fmod */

#include "BeatTick.h"

using namespace base;

session::Clip::Clip(const allocator_type& alloc) noexcept :
    name(alloc), noteEvents(alloc), parameterEvents(alloc), m_activeNotes(alloc)
{
}

session::Clip::Clip(Clip&& other, const allocator_type& alloc) noexcept :
    name(std::move(other.name), alloc),
    noteEvents(std::move(other.noteEvents), alloc),
    parameterEvents(std::move(other.parameterEvents), alloc),
    m_activeNotes(std::move(other.m_activeNotes), alloc)
{
}

session::Clip::Clip(const Clip& other, const allocator_type& alloc) :
    name(other.name, alloc),
    noteEvents(other.noteEvents, alloc),
    parameterEvents(other.parameterEvents, alloc),
    m_activeNotes(alloc)
{
}

void session::Clip::update(instruments::Instrument* instrument)
{
   sequencer::Beat clipBeat = tempo::BeatTick::instance().getBeat() - startTime;
   clipBeat                 = std::fmod(clipBeat, sequenceLength);
   for (auto it = m_activeNotes.begin(); it != m_activeNotes.end();)
   {
      const auto endStamp =
          std::fmod(((*it)->beatstamp + (*it)->duration), sequenceLength);
      if (clipBeat >= endStamp)
      {
         if (instrument)
         {
            instrument->noteOff((*it)->note, (*it)->velocity);
         }
         it = m_activeNotes.erase(it);
      }
      else
      {
         ++it;
      }
   }
   noteEvents.forNoteEvents(
       prevClipBeat, clipBeat,
       [this, instrument](const auto& noteEventIt) {
          if (instrument)
          {
             instrument->noteOn(noteEventIt->second.note, noteEventIt->second.velocity);
          }
          m_activeNotes.push_back(&noteEventIt->second);
       });
   prevClipBeat = clipBeat;
}

void session::Clip::start()
{

}

void session::Clip::stop()
{
   
}
