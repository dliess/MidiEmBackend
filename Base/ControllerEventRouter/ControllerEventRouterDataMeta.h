#ifndef CONTROLLER_EVENT_ROUTER_DATA_META_H
#define CONTROLLER_EVENT_ROUTER_DATA_META_H

#include <nlohmann/json.hpp>

#include "ControllerEventRouterData.h"
namespace base::eventRouter
{
inline void to_json(nlohmann::json& j, const Data& obj)
{
   for (const auto& [from, to] : obj)
   {
      nlohmann::json both;
      both["from"] = nlohmann::json(from);
      both["to"]   = nlohmann::json(to);
      j.push_back(both);
   }
}

inline void from_json(const nlohmann::json& j, Data& obj)
{
   for (const auto& entry : j)
   {
      obj.try_emplace(entry["from"].get<Data::key_type>(),
                      entry["to"].get<Data::mapped_type>());
   }
}

}   // namespace base::eventRouter

#endif