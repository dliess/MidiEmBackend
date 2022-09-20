#ifndef INSTRUMENTS_META_H
#define INSTRUMENTS_META_H

#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::instruments::Instruments::Data>()
{
   return members(
      member("kitInstruments", &base::instruments::Instruments::Data::kitInstruments),
      member("melodicInstruments",
             &base::instruments::Instruments::Data::melodicInstruments));
}

} // namespace meta

#endif