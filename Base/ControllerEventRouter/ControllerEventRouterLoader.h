#ifndef BASE_EVENT_ROUTE_EVENT_ROUTER_LOADER_H
#define BASE_EVENT_ROUTE_EVENT_ROUTER_LOADER_H

#include <unordered_map>

#include "CallbackSignal.h"
#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "MusicDeviceId.h"
#include "Settings.h"

namespace base::musicDevice
{
class MusicDeviceContainer;
}
namespace base::eventRouter::loader
{
struct EventIdExt
{
   musicDevice::MusicDeviceId mdId;
   musicDevice::controller::EventId eventId;
};

inline bool operator==(const EventIdExt& lhs, const EventIdExt& rhs)
{
   return lhs.mdId == rhs.mdId && lhs.eventId == rhs.eventId;
}

struct EventDestinationL
{
   musicDevice::MusicDeviceId mdId;
   int voiceIdx;
   using ControlType =
       mpark::variant<mpark::monostate, EventDestination::Note, EventDestination::Parameter>;
   ControlType controlType;
};

inline bool operator==(const EventDestinationL& lhs,
                       const EventDestinationL& rhs)
{
   return lhs.mdId == rhs.mdId && lhs.voiceIdx == rhs.voiceIdx &&
          lhs.controlType == rhs.controlType;
}

class EventRoutes
{
public:
   EventRoutes();
   void loadFromFile();
   void musicDeviceAppeared(const musicDevice::MusicDeviceId& mdId);
   void musicDeviceDisappeared(const musicDevice::MusicDeviceId& mdId);
   void connectNotes2Notes(const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
                           int note, int eventIdx, int channelIdx,
                           const musicDevice::MusicDeviceId& soundDevID, int voiceIdx);
   void connectNotes2Parameter(const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
                               int note, int eventIdx, int channelIdx,
                               const musicDevice::MusicDeviceId& soundDevID, int voiceIdx,
                               int parameterIdx,
                               ParameterDestination paramFunc);
   void connectWidget2Notes(const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
                            int widgetCoordX, int widgetCoordY, int eventIdx,
                            int channelIdx, const musicDevice::MusicDeviceId& soundDevID,
                            int voiceIdx);
   void connectWidget2Parameter(const musicDevice::MusicDeviceId& controllerID,
                                int widgetIdx, int widgetCoordX,
                                int widgetCoordY, int eventIdx, int channelIdx,
                                const musicDevice::MusicDeviceId& soundDevID, int voiceIdx,
                                int parameterIdx,
                                ParameterDestination paramFunc);
   void eraseConnectionForNotes(const musicDevice::MusicDeviceId& controllerID,
                                int widgetIdx, int note, int eventIdx,
                                int channelIdx);
   void eraseConnectionForWidget(const musicDevice::MusicDeviceId& controllerID,
                                 int widgetIdx, int widgetCoordX,
                                 int widgetCoordY, int eventIdx,
                                 int channelIdx);
   void eraseConnectionsToDestinationNotes(const musicDevice::MusicDeviceId& soundDevID,
                                           int voiceIdx);
   void eraseConnectionsToDestinationParameter(const musicDevice::MusicDeviceId& soundDevID,
                                               int voiceIdx, int parameterIdx,
                                               ParameterDestination paramFunc);

   CB_SIGNAL(ConnectionLoadedNotes2Notes, const musicDevice::MusicDeviceId& controllerID,
             int widgetIdx, int note, int eventIdx, int channelIdx,
             const musicDevice::MusicDeviceId& soundDevID, int voiceIdx);
   CB_SIGNAL(ConnectionLoadedNotes2Parameter, const musicDevice::MusicDeviceId& controllerID,
             int widgetIdx, int note, int eventIdx, int channelIdx,
             const musicDevice::MusicDeviceId& soundDevID, int voiceIdx, int parameterIdx,
             ParameterDestination paramFunc);
   CB_SIGNAL(ConnectionLoadedWidget2Notes, const musicDevice::MusicDeviceId& controllerID,
             int widgetIdx, int widgetCoordX, int widgetCoordY, int eventIdx,
             int channelIdx, const musicDevice::MusicDeviceId& soundDevID, int voiceIdx);
   CB_SIGNAL(ConnectionLoadedWidget2Parameter,
             const musicDevice::MusicDeviceId& controllerID, int widgetIdx, int widgetCoordX,
             int widgetCoordY, int eventIdx, int channelIdx,
             const musicDevice::MusicDeviceId& soundDevID, int voiceIdx, int parameterIdx,
             ParameterDestination paramFunc);

   CB_SIGNAL(ConnectionUnloadedNotes, const musicDevice::MusicDeviceId& controllerID,
             int widgetIdx, int note, int eventIdx, int channelIdx);
   CB_SIGNAL(ConnectionUnloadedWidget, const musicDevice::MusicDeviceId& controllerID,
             int widgetIdx, int widgetCoordX, int widgetCoordY, int eventIdx,
             int channelIdx);

   struct MapEntry
   {
      EventIdExt from;
      EventDestinationL to;
   };

private:
   std::vector<MapEntry> m_data;
   void emitEntry(const MapEntry& mapEntry);
   void emitEntryGotDisabled(const MapEntry& e);
   util::Settings m_settings;
   void insert(const EventIdExt& from, const EventDestinationL& to);
   void erase(const EventIdExt& from);
   static const std::string CONFIG_SECTION;
};

}   // namespace base::eventRouter

#include "ControllerEventRouterLoaderMeta.h"

#endif