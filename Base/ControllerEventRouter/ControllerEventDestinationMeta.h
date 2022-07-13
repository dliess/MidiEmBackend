#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_META_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_META_H

#include "JsonCast.h"

template <>
inline void to_json<base::musicDevice::controller::ParameterDestination>(
    nlohmann::json& j,
    const base::musicDevice::controller::ParameterDestination& obj)
{
   j = ~obj;
}

template <>
inline void from_json<base::musicDevice::controller::ParameterDestination>(
    const nlohmann::json& j,
    base::musicDevice::controller::ParameterDestination& obj)
{
   obj = base::musicDevice::controller::create_ParameterDestination(j.get<std::string>());
}

#endif
