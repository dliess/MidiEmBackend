#include "Clip.h"

#include <math.h> /* fmod */

#include "BeatTick.h"

using namespace base;

session::Clip::Clip(const allocator_type& alloc) noexcept :
    m_name(alloc),
    m_noteEvents(alloc),
    m_parameterEvents(alloc),
    m_activeNotes(alloc)
{
}

session::Clip::Clip(Clip&& other, const allocator_type& alloc) noexcept :
    m_name(std::move(other.m_name), alloc),
    m_noteEvents(std::move(other.m_noteEvents), alloc),
    m_parameterEvents(std::move(other.m_parameterEvents), alloc),
    m_activeNotes(std::move(other.m_activeNotes), alloc)
{
}

session::Clip::Clip(const Clip& other, const allocator_type& alloc) :
    m_name(other.m_name, alloc),
    m_noteEvents(other.m_noteEvents, alloc),
    m_parameterEvents(other.m_parameterEvents, alloc),
    m_activeNotes(alloc)
{
}

void session::Clip::update(const instruments::Instrument* instrument)
{
   sequencer::Beat clipBeat =
       tempo::BeatTick::instance().getLocalBeat() - m_startBeat;
   clipBeat = std::fmod(clipBeat, m_sequenceLength);
   for (auto it = m_activeNotes.begin(); it != m_activeNotes.end();)
   {
      const auto endStamp =
          std::fmod(((*it)->beatstamp + (*it)->length), m_sequenceLength);
      const bool noteOverflow = (*it)->beatstamp > endStamp;
      const bool turnNoteOff =
          noteOverflow ? (endStamp < clipBeat && clipBeat < (*it)->beatstamp)
                       : (clipBeat >= endStamp);
      if (turnNoteOff)
      {
         if (instrument)
         {
            instrument->noteOff((*it)->note, (*it)->velocity, this);
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
                           noteEventIt->second.velocity, this);
          }
          m_activeNotes.push_back(&noteEventIt->second);
       });
   m_prevClipBeat = clipBeat;
}

void session::Clip::reset()
{
   m_startBeat = static_cast<long>(tempo::BeatTick::instance().getLocalBeat());
   m_prevClipBeat = m_startBeat;
}

void session::Clip::stop(const instruments::Instrument* instrument)
{
   for (auto& activeNote : m_activeNotes)
   {
      if (instrument)
      {
         instrument->noteOff(activeNote->note, activeNote->velocity, this);
      }
   }
   m_activeNotes.clear();
}
