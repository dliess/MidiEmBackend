#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_RT_INL
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_RT_INL

#include "MelodicInstrumentVoice.h"

namespace base::instruments::rt
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

}   // namespace base::instruments::rt

#endif
