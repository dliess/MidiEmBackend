#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_INL
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_INL

#include "MelodicInstrumentVoice.h"

namespace base::instruments
{
inline MelodicVoice::MelodicVoice(std::string name) noexcept :
    name(std::move(name))
{
}

inline
bool MelodicVoice::operator==(const MelodicVoice& rhs) const
{
    return name == rhs.name &&
           components == rhs.components;
}

}   // namespace base::instruments

#endif