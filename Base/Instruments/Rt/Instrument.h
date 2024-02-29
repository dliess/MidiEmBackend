#ifndef BASE_INSTRUMENT_RT_H
#define BASE_INSTRUMENT_RT_H

#include "Identifiable.h"
#include "CallbackSignal.h"
namespace base::instruments::rt {
class Instrument : public util::Identifiable
{
public:
   Instrument(util::Identifiable::UUIDView uuid) : util::Identifiable(uuid) {}
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOnPlayed, int, float, void*);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOffPlayed, int, float, void*);
   float externalAmp{1.0f};
};
} // namespace base::instruments::rt

#endif
