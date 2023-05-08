#ifndef MUSIC_DEVICE_DESCRIPTION_META_H
#define MUSIC_DEVICE_DESCRIPTION_META_H

#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::musicDevice::description::Description>()
{
   return members(
      member("version", &base::musicDevice::description::Description::version),
      member("manufacturer", &base::musicDevice::description::Description::manufacturer),
      member("productName", &base::musicDevice::description::Description::productName),
      member("soundSection", &base::musicDevice::description::Description::soundSection),
      member("controllerSection", &base::musicDevice::description::Description::controllerSection),
      member("hubSection", &base::musicDevice::description::Description::hubSection),
      member("reactsToTransportCommands",
             &base::musicDevice::description::Description::reactsToTransportCommands));
}

} // namespace meta

#endif