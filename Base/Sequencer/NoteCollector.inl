#include <algorithm>
#include <ranges>

#include "BeatTick.h"
#include "NoteCollector.h"

namespace base::sequencer
{

inline
void NoteCollector::noteOn(int note, float velocity)
{
   m_notes.at(note) = {velocity, base::tempo::BeatTick::instance().getBeat()};
}

inline
void NoteCollector::noteOff(int note, float velocity)
{
   if (m_notes.at(note))
   {
      const auto& n = m_notes.at(note);
      const Beat duration =
          base::tempo::BeatTick::instance().getBeat() - n->startBeat;
      emitNoteOccured(Note{n->startBeat, note, n->velocity, duration});
      m_notes.at(note).reset();
   }
}

inline
void NoteCollector::reset() noexcept
{
   std::ranges::fill(m_notes, std::nullopt);
}

}   // namespace base::sequencer
