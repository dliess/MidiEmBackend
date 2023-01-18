#ifndef MUSIC_DEVICE_META_H
#define MUSIC_DEVICE_META_H

#include "JsonCast.h"

template<>
inline void to_json<base::musicDevice::MusicDevice>(
   nlohmann::json& j,
   const base::musicDevice::MusicDevice& obj)
{
    j["id"] = meta::serialize(obj.id());
    j["deviceName"] = meta::serialize(obj.deviceId().deviceName());
    j["portName"] = meta::serialize(obj.deviceId().portName());
    j["soundHandler"] = static_cast<bool>(obj.soundHandler);
    j["controllerHandler"] = static_cast<bool>(obj.controllerHandler);
    j["sequencer"] = static_cast<bool>(obj.sequencer);
}

#endif