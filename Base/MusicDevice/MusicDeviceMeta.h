#ifndef MUSIC_DEVICE_META_H
#define MUSIC_DEVICE_META_H

#include "JsonCast.h"

template<>
inline void to_json<base::musicDevice::MusicDevice>(
   nlohmann::json& j,
   const base::musicDevice::MusicDevice& obj)
{
    j["deviceId"] = meta::serialize(obj.deviceId());
    j["id"] = meta::serialize(obj.id());
    if(obj.soundHandler) j["soundHandler"] = true;
    if(obj.controllerHandler) j["controllerHandler"] = true;
    if(obj.sequencer) j["sequencer"] = true;
}

#endif