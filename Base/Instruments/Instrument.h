#ifndef BASE_INSTRUMENT_H
#define BASE_INSTRUMENT_H

#include "Identifiable.h"
#include "INotePlayer.h"
#include "CallbackSignal.h"
#include <memory>
namespace base::instruments
{
class Instrument : public util::Identifiable, public INotePlayer
{
public:
   void markAsDefaultCreated() noexcept { m_isDefaultCreated = true; };
   void unmarkAsDefaultCreated() noexcept { m_isDefaultCreated = false; };
   bool isDefaultCreated() const noexcept { return m_isDefaultCreated; }
   int refCount() const { return m_refCount; }
   void incRefCount() { ++m_refCount; }
   void decRefCount() { if(m_refCount > 0) --m_refCount; }
   struct RtNoteData {
      CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOnPlayed, int, float, void*);
      CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOffPlayed, int, float, void*);
      float externalAmp{1.0f};
      friend class KitInstrument;
      friend class MelodicInstrument;
   };
   std::shared_ptr<RtNoteData> rtData{std::make_shared<RtNoteData>()};
protected:
   bool m_isDefaultCreated{false};
   int m_refCount{0};
};

} // namespace base::instruments
#endif
