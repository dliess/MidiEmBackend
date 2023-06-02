#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_META_H
#define BASE_INSTRUMENTS_PARAMETER_CACHE_META_H

#include <JsonCast.h>
#include "InstrumentsParameterCache.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::ParameterCache>()
{
   return members(member("data", &base::instruments::ParameterCache::nonRtBackupData_));
}

} // namespace meta

#endif