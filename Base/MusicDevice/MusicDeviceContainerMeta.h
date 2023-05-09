#ifndef MUSIC_DEVICE_CONTAINER_META_H
#define MUSIC_DEVICE_CONTAINER_META_H

#include "JsonCast.h"

namespace base::musicDevice
{
inline void to_json(nlohmann::json& j, const MusicDeviceContainer& obj)
{
   j = nlohmann::json::array();
   for (auto& it : obj) { j.push_back(nlohmann::json(*it.second.get())); }
}

}   // namespace base::musicDevice
#endif