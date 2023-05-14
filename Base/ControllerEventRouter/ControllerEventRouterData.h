#ifndef CONTROLLER_EVENT_ROUTER_DATA_H
#define CONTROLLER_EVENT_ROUTER_DATA_H

#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "MusicDeviceId.h"
namespace base::eventRouter
{
template<typename ControllerIdType>
using MapDataType =
    std::unordered_map<::base::musicDevice::controller::EventIdAndUuid<ControllerIdType>, EventDestination>;

using Data = MapDataType<util::Identifiable::UUID>;
using LoaderData = MapDataType<musicDevice::MusicDeviceId>;

inline
void initRtCache(Data& data)
{
    for(auto& e: data)
    {
        initRtCache(e.second);
    }
}
}   // namespace base::eventRouter

#include "ControllerEventRouterDataMeta.h"

#endif