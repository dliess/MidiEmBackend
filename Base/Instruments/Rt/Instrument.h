#ifndef BASE_INSTRUMENT_RT_H
#define BASE_INSTRUMENT_RT_H

#include "Identifiable.h"
#include "CallbackSignal.h"
#include <memory>
namespace base::instruments
{

namespace rt {
class Instrument : public util::Identifiable
{
public:
   void noteOn(int note, float velocity, void* token) const;
   void noteOff(int note, float velocity, void* token) const;
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOnPlayed, int, float, void*);
   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOffPlayed, int, float, void*);
   float externalAmp{1.0f};
};
} // namespace rt

} // namespace base::instruments
#endif
