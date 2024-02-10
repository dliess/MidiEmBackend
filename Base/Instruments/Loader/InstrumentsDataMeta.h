#ifndef INSTRUMENTS_DATA_META_LOADER_H
#define INSTRUMENTS_DATA_META_LOADER_H

#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::instruments::loader::Data>()
{
   return members(
      member("kitInstruments", &base::instruments::loader::Data::kitInstruments),
      member("melodicInstruments",
             &base::instruments::loader::Data::melodicInstruments));
}

} // namespace meta

#endif
