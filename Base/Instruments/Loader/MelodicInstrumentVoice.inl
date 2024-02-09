#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_LOADER_INL
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_LOADER_INL

#include "MelodicInstrumentVoice.h"

namespace base::instruments::loader
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

}   // namespace base::instruments::loader

#endif
