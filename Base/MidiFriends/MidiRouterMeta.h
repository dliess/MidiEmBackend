#ifndef MIDI_ROUTER_META_H
#define MIDI_ROUTER_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace meta
{

template <>
inline auto registerMembers<base::midifriends::RoutingDataSpecialized>()
{
   return members(
      member("transmitClockMsg", &base::midifriends::RoutingDataSpecialized::transmitClockMsg),
      member("channelMapping", &base::midifriends::RoutingDataSpecialized::channelMapping)
   );
}

template <>
inline auto registerMembers<base::midifriends::RoutingData>()
{
   return members(
      member("specialized", &base::midifriends::RoutingData::specialized)
   );
}

} // namespace meta

#endif