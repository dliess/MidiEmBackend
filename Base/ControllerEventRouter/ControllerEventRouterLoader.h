#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_H

#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "CallbackSignal.h"

#include "MusicDeviceId.h"
#include "Settings.h"

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

inline bool operator==(const EventIdExt& lhs, const EventIdExt& rhs)
{
   return lhs.mdId == rhs.mdId && lhs.eventId == rhs.eventId;
}

struct EventDestinationL
{
   MusicDeviceId mdId;
   int voiceIdx;
   using Endpoint = mpark::variant<mpark::monostate, EventDestination::Note, EventDestination::ParameterBase>;
   Endpoint endpoint;
};

class EventRoutes
{
public:
   EventRoutes();
   void loadFromFile();
   void connectedNotes2Notes(
            const MusicDeviceId& controllerID,
            int widgetIdx,
            int note,
            int eventIdx, 
            int channelIdx,
            const MusicDeviceId& soundDevID,
            int voiceIdx);
   void connectedNotes2Parameter(
            const MusicDeviceId& controllerID,
            int widgetIdx,
            int note,
            int eventIdx, 
            int channelIdx,
            const MusicDeviceId& soundDevID,
            int voiceIdx,
            int parameterIdx,
            ParameterDestination paramFunc);
   void connectedWidget2Notes(
            const MusicDeviceId& controllerID,
            int widgetIdx,
            int widgetCoordX,
            int widgetCoordY,
            int eventIdx, 
            int channelIdx,
            const MusicDeviceId& soundDevID,
            int voiceIdx);
   void connectedWidget2Parameter(
            const MusicDeviceId& controllerID,
            int widgetIdx,
            int widgetCoordX,
            int widgetCoordY,
            int eventIdx, 
            int channelIdx,
            const MusicDeviceId& soundDevID,
            int voiceIdx,
            int parameterIdx,
            ParameterDestination paramFunc);
   struct MapEntry
   {
      EventIdExt from;
      EventDestinationL to;
   };

private:
   std::vector<MapEntry> m_data;
   util::Settings m_settings;
   void insert(const EventIdExt from, const EventDestinationL& to);
   static const std::string CONFIG_SECTION;
};

}   // namespace controller
}   // namespace base::musicDevice

#include "ControllerEventRouterLoaderMeta.h"

#endif