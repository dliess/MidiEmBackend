#ifndef INSTRUMENTS_DATA_META_H
#define INSTRUMENTS_DATA_META_H

#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::instruments::Data>()
{
   return members(
      member("kitInstruments", &base::instruments::Data::kitInstruments),
      member("melodicInstruments",
             &base::instruments::Data::melodicInstruments));
}

} // namespace meta

#endif