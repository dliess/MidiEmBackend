#ifndef INSTRUMENTS_KIT_INSTRUMENTS_VOICE_LOADER_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_VOICE_LOADER_INL

#include "KitInstrumentVoice.h"

namespace base::instruments::loader {

inline 
KitVoice::KitVoice(std::string name) noexcept :
    name(std::move(name))
{
}

inline
bool KitVoice::operator==(const KitVoice& rhs) const
{
    return name == rhs.name &&
           components == rhs.components;
}

}   // namespace base::instruments::loader

#endif
