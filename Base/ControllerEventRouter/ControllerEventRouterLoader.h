#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H

//#include <map>
#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "CallbackSignal.h"

#include "MusicDeviceId.h"

namespace base::musicDevice
{
class MusicDeviceContainer;

namespace controller::loader
{

struct EventIdExt
{
   MusicDeviceId mdId;
   EventId eventId;
};


class EventRouter
{
public:
   EventRouterLoader();
private:
   std::unordered_map<EventIdExt, EventDestination> m_map;
};

}   // namespace controller
}   // namespace base::musicDevice
#endif