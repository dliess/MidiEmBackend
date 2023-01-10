#ifndef BASE_EVENT_ROUTE_EVENT_DESTINATION_META_H
#define BASE_EVENT_ROUTE_EVENT_DESTINATION_META_H

#include "JsonCast.h"

template <>
inline void to_json<base::eventRouter::ParameterDestination>(
    nlohmann::json& j,
    const base::eventRouter::ParameterDestination& obj)
{
   j = ~obj;
}

template <>
inline void from_json<base::eventRouter::ParameterDestination>(
    const nlohmann::json& j,
    base::eventRouter::ParameterDestination& obj)
{
   obj = base::eventRouter::create_ParameterDestination(j.get<std::string>());
}

#endif
