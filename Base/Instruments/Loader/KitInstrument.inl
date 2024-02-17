#ifndef INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL

#include "KitInstrument.h"

namespace base::instruments::loader
{

inline bool isSameInstrument(const KitInstrument& lhs, const KitInstrument& rhs)
{
   return lhs.m_name == rhs.m_name && lhs.m_voices == rhs.m_voices;
}

}   // namespace base::instruments

#endif
