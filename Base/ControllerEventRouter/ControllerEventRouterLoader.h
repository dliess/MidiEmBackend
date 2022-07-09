#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H

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

struct EventDestinationL
{
   MusicDeviceId mdId;
   int voiceIdx;
   using Endpoint = mpark::variant<mpark::monostate, EventDestination::Note, EventDestination::ParameterBase>;
   Endpoint endpoint;
};



class EventRouter
{
public:
private:
   std::unordered_map<EventIdExt, EventDestinationL> m_map;
};

}   // namespace controller
}   // namespace base::musicDevice
#endif