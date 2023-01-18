#ifndef MUSIC_DEVICE_ID_META_H
#define MUSIC_DEVICE_ID_META_H

#include <string>
#include <Meta.h>
#include <JsonCast.h>

#include "MusicDeviceJsonNamespaceFix.h"

namespace meta
{

template <>
inline auto registerMembers<base::musicDevice::MusicDeviceId>()
{
   return members(
      member("deviceName", &base::musicDevice::MusicDeviceId::m_deviceName),
      member("portName",   &base::musicDevice::MusicDeviceId::m_portName)
   );
}

} // namespace meta

namespace std
{
  template <>
  struct hash<base::musicDevice::MusicDeviceId>
  {
    size_t operator()(const base::musicDevice::MusicDeviceId& rMusicDevice) const noexcept
    {
        hash<string> hasher;
        return hasher(rMusicDevice.deviceName()) ^ hasher(rMusicDevice.portName());
    }
  };
} // namespace std

#endif