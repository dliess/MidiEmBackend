#ifndef CONTROLLER_EVENT_ROUTER_DATA_META_H
#define CONTROLLER_EVENT_ROUTER_DATA_META_H

#include "ControllerEventRouterData.h"

#include <nlohmann/json.hpp>
namespace base::eventRouter
{

inline void to_json(nlohmann::json& j, const Data& obj) { assert(false); }

inline void from_json(const nlohmann::json& j, Data& obj) { assert(false); }

}   // namespace base::eventRouter


#endif