#include "Clip.h"

#include <math.h> /* fmod */

#include "BeatTick.h"

using namespace base;

session::Clip::Clip(const allocator_type& alloc) noexcept :
    name(alloc),
    m_noteEvents(alloc),
    m_parameterEvents(alloc),
    m_activeNotes(alloc)
{
}

session::Clip::Clip(Clip&& other, const allocator_type& alloc) noexcept :
    name(std::move(other.name), alloc),
    m_noteEvents(std::move(other.m_noteEvents), alloc),
    m_parameterEvents(std::move(other.m_parameterEvents), alloc),
    m_activeNotes(std::move(other.m_activeNotes), alloc)
{
}

session::Clip::Clip(const Clip& other, const allocator_type& alloc) :
    name(other.name, alloc),
    m_noteEvents(other.m_noteEvents, alloc),
    m_parameterEvents(other.m_parameterEvents, alloc),
    m_activeNotes(alloc)
{
}

void session::Clip::update(instruments::Instrument* instrument)
{
   sequencer::Beat clipBeat =
       tempo::BeatTick::instance().getBeat() - m_startBeat;
   clipBeat = std::fmod(clipBeat, m_sequenceLength);
   for (auto it = m_activeNotes.begin(); it != m_activeNotes.end();)
   {
      const auto endStamp =
          std::fmod(((*it)->beatstamp + (*it)->duration), m_sequenceLength);
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
   m_noteEvents.forNoteEvents(
       m_prevClipBeat, clipBeat, [this, instrument](const auto& noteEventIt) {
          if (instrument)
          {
             instrument->noteOn(noteEventIt->second.note,
                                noteEventIt->second.velocity);
          }
          m_activeNotes.push_back(&noteEventIt->second);
       });
   m_prevClipBeat = clipBeat;
}

void session::Clip::start()
{
   m_startBeat = static_cast<long>(tempo::BeatTick::instance().getBeat());
   m_prevClipBeat = m_startBeat;
}

void session::Clip::stop(instruments::Instrument* instrument)
{
   for (auto& activeNote : m_activeNotes)
   {
      if (instrument)
      {
         instrument->noteOff(activeNote->note, activeNote->velocity);
      }
   }
   m_activeNotes.clear();
}
