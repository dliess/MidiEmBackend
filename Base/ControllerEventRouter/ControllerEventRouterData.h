#ifndef CONTROLLER_EVENT_ROUTER_DATA_H
#define CONTROLLER_EVENT_ROUTER_DATA_H

#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"

namespace base::eventRouter
{
using Data =
    std::unordered_map<musicDevice::controller::EventIdExt, EventDestination>;

}   // namespace base::eventRouter

#include "ControllerEventRouterDataMeta.h"

#endif