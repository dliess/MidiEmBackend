#ifndef MELODIC_INSTRUMENT_LOADER_INL
#define MELODIC_INSTRUMENT_LOADER_INL

#include "MelodicInstrument.h"

namespace base::instruments::loader
{

inline
bool isSameInstrument(const MelodicInstrument& lhs,
                      const MelodicInstrument& rhs)
{
   return lhs.m_name == rhs.m_name;
}

}   // namespace base::instruments::loader

#endif   // MELODIC_INSTRUMENT_LOADER_INL
