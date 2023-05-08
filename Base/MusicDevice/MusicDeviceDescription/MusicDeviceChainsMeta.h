#ifndef MUSIC_DEVICE_CHAINS_META_H
#define MUSIC_DEVICE_CHAINS_META_H

#include "MusicDeviceChains.h"
#include "Meta.h"

#include <JsonCast.h>

namespace meta
{

template<>
inline auto registerMembers<base::musicDevice::DeviceChainsDeviceDescription>()
{
   return members(
      member("musicDeviceName", &base::musicDevice::DeviceChainsDeviceDescription::musicDeviceName),
      member("midiVoiceOffset", &base::musicDevice::DeviceChainsDeviceDescription::midiVoiceOffset)
   );
}

template<>
inline auto registerMembers<base::musicDevice::DeviceChains>()
{
   return members(
      member("deviceChains", &base::musicDevice::DeviceChains::deviceChains)
   );
}

} // namespace meta


#endif