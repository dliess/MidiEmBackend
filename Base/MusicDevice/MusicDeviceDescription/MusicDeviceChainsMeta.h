#ifndef MUSIC_DEVICE_CHAINS_META_H
#define MUSIC_DEVICE_CHAINS_META_H

#include "MusicDeviceChains.h"
#include "Meta.h"

#include <JsonCast.h>
#include "MusicDeviceJsonNamespaceFix.h"

namespace meta
{

template<>
inline auto registerMembers<base::musicDevice::DeviceChains>()
{
   return members(
      member("inputChains", &base::musicDevice::DeviceChains::deviceChains)
   );
}

} // namespace meta


#endif