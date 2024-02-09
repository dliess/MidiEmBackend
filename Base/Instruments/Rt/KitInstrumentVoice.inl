#ifndef INSTRUMENTS_KIT_INSTRUMENTS_RT_LOADER_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_RT_LOADER_INL

#include "KitInstrumentVoice.h"

namespace base::instruments::rt
{
{
inline KitVoice::KitVoice(std::string name) noexcept :
    name(std::move(name))
{
}

inline
bool KitVoice::operator==(const KitVoice& rhs) const
{
    return name == rhs.name &&
           components == rhs.components;
}


}   // namespace base::instruments::rt

#endif
