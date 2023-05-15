#ifndef BASE_SESSION_NOTE_COLLECTOR_H
#define BASE_SESSION_NOTE_COLLECTOR_H

#include <array>
#include <optional>

#include "CallbackSignal.h"
#include "NoteEvent.h"

namespace base::sequencer
{
class NoteCollector
{
public:
   void noteOn(int note, float velocity);
   void noteOff(int note, float velocity);
   void reset() noexcept;
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOccured, const Note&);
private:
   static constexpr int NUM_NOTES = 128;
   struct StartNoteEvt
   {
       float velocity{1.0};
       Beat startBeat{0.0};
   };
   std::array<std::optional<StartNoteEvt>, NUM_NOTES> m_notes{};
};

}   // namespace base::sequencer

#include "NoteCollector.inl"
#endif