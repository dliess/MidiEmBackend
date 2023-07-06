#ifndef SYSEX_DESCRIPTION_META_H
#define SYSEX_DESCRIPTION_META_H

#include "JsonCast.h"
#include "Meta.h"
#include "SysExDescription.h"

namespace meta
{
template <>
inline auto registerMembers<base::musicDevice::description::midisysex::Bytes>()
{
   return members(member(
       "values", &base::musicDevice::description::midisysex::Bytes::values));
}

template <>
inline auto
getClassNameOrIndex<base::musicDevice::description::midisysex::Bytes>(
    int i) noexcept
{
   return "Bytes";
}

} // namespace meta

#endif