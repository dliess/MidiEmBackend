#ifndef CONTROLLER_EVENT_ROUTER_DATA_META_H
#define CONTROLLER_EVENT_ROUTER_DATA_META_H

#include <nlohmann/json.hpp>
#include "JsonCast.h"
#include "MusicDeviceId.h"

#include "ControllerEventRouterData.h"

using EventIdAndUuidLoader = base::musicDevice::controller::EventIdAndUuid<base::musicDevice::MusicDeviceId>;

namespace std
{
template <> struct hash<EventIdAndUuidLoader>
{
   std::size_t operator()(
       const EventIdAndUuidLoader& event) const
   {
      return std::hash<base::musicDevice::MusicDeviceId>{}(event.uuid) ^
             std::hash<base::musicDevice::controller::EventId>{}(event.eventId);
   }
};

}

namespace meta
{
template <> inline auto registerMembers<EventIdAndUuidLoader>()
{
   return members(
       member("uuid", &EventIdAndUuidLoader::uuid),
       member("eventId", &EventIdAndUuidLoader::eventId));
}

}   // namespace meta

namespace base::eventRouter
{
inline void to_json(nlohmann::json& j, const LoaderData& obj)
{
   for (const auto& [from, to] : obj)
   {
      nlohmann::json both;
      both["from"] = nlohmann::json(from);
      both["to"]   = nlohmann::json(to);
      j.push_back(both);
   }
}

inline void from_json(const nlohmann::json& j, LoaderData& obj)
{
   for (const auto& entry : j)
   {
      obj.try_emplace(entry["from"].get<LoaderData::key_type>(),
                      entry["to"].get<LoaderData::mapped_type>());
   }
}

}   // namespace base::eventRouter


#endif