#ifndef MUSIC_DEVICE_CONTAINER_META_H
#define MUSIC_DEVICE_CONTAINER_META_H

#include "JsonCast.h"

template<>
inline void to_json<base::musicDevice::MusicDeviceContainer>(
   nlohmann::json& j,
   const base::musicDevice::MusicDeviceContainer& obj)
{
   j = nlohmann::json::array();
   for(auto& it : obj)
   {
      j.push_back(meta::serialize(it.second.get()));
   }
}

#endif