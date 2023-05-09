#ifndef MUSIC_DEVICE_META_H
#define MUSIC_DEVICE_META_H

#include "JsonCast.h"

namespace base::musicDevice
{
inline void to_json(nlohmann::json& j, const MusicDevice& obj)
{
   j["id"]                = nlohmann::json(obj.id());
   j["deviceName"]        = nlohmann::json(obj.deviceId().deviceName());
   j["portName"]          = nlohmann::json(obj.deviceId().portName());
   j["soundHandler"]      = static_cast<bool>(obj.soundHandler);
   j["controllerHandler"] = static_cast<bool>(obj.controllerHandler);
   j["sequencer"]         = static_cast<bool>(obj.sequencer);
}

}   // namespace base::musicDevice

#endif