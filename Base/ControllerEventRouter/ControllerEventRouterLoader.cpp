#include "ControllerEventRouterLoader.h"

#include "ControllerHandler.h"
#include "MusicDeviceContainer.h"

using namespace base::musicDevice::controller::loader;

const std::string EventRoutes::CONFIG_SECTION = "ControllerEventRoutes";

EventRoutes::EventRoutes() :
   m_settings("ControllerEventRoutes", "ControllerEventRoutes.json")
{
}

void EventRoutes::loadFromFile()
{
   m_data = m_settings.load<decltype(m_data)>(CONFIG_SECTION);
}

void EventRoutes::connectedNotes2Notes(const MusicDeviceId& controllerID,
                                       int widgetIdx, int note, int eventIdx,
                                       int channelIdx,
                                       const MusicDeviceId& soundDevID,
                                       int voiceIdx)
{
   const EventIdExt from{controllerID,
                         EventId{widgetIdx, Note{note}, eventIdx, channelIdx}};
   const EventDestinationL to{soundDevID, voiceIdx, mpark::monostate()};
   insert(from, to);
}

void EventRoutes::connectedNotes2Parameter(const MusicDeviceId& controllerID,
                                           int widgetIdx, int note,
                                           int eventIdx, int channelIdx,
                                           const MusicDeviceId& soundDevID,
                                           int voiceIdx, int parameterIdx,
                                           ParameterDestination paramFunc)
{
   const EventIdExt from{controllerID,
                         EventId{widgetIdx, Note{note}, eventIdx, channelIdx}};
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   insert(from, to);
}

void EventRoutes::connectedWidget2Notes(const MusicDeviceId& controllerID,
                                        int widgetIdx, int widgetCoordX,
                                        int widgetCoordY, int eventIdx,
                                        int channelIdx,
                                        const MusicDeviceId& soundDevID,
                                        int voiceIdx)
{
   const EventIdExt from{
       controllerID, EventId{widgetIdx, WidgetCoord{widgetCoordX, widgetCoordY},
                             eventIdx, channelIdx}};
   const EventDestinationL to{soundDevID, voiceIdx, mpark::monostate()};
   insert(from, to);
}

void EventRoutes::connectedWidget2Parameter(const MusicDeviceId& controllerID,
                                            int widgetIdx, int widgetCoordX,
                                            int widgetCoordY, int eventIdx,
                                            int channelIdx,
                                            const MusicDeviceId& soundDevID,
                                            int voiceIdx, int parameterIdx,
                                            ParameterDestination paramFunc)
{
   const EventIdExt from{
       controllerID, EventId{widgetIdx, WidgetCoord{widgetCoordX, widgetCoordY},
                             eventIdx, channelIdx}};
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   insert(from, to);
}

void EventRoutes::insert(const EventIdExt from, const EventDestinationL& to)
{
   auto it =
       std::find_if(m_data.begin(), m_data.end(),
                    [&from](const MapEntry& e) { return e.from == from; });
   if (it != m_data.end())
   {
      m_data.erase(it);
   }
   m_data.push_back(MapEntry{from, to});
   m_settings.save(CONFIG_SECTION, m_data);
}