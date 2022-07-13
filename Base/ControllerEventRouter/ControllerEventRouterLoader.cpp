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
   for (const auto& e : m_data) { emitEntry(e); }
}

void EventRoutes::emitEntry(const MapEntry& e)
{
   mpark::visit(
       util::overload{
           [&e, this](const WidgetCoord& wc) {
              mpark::visit(
                  util::overload{
                      [&e, &wc, this](const EventDestination::Note& note) {
                         emitConnectionLoadedWidget2Notes(
                             e.from.mdId, e.from.eventId.widgetId, wc.col,
                             wc.row, e.from.eventId.eventId,
                             e.from.eventId.channelId, e.to.mdId,
                             e.to.voiceIdx);
                      },
                      [&e, &wc,
                       this](const EventDestination::ParameterBase& param) {
                         emitConnectionLoadedWidget2Parameter(
                             e.from.mdId, e.from.eventId.widgetId, wc.col,
                             wc.row, e.from.eventId.eventId,
                             e.from.eventId.channelId, e.to.mdId, e.to.voiceIdx,
                             param.id, param.parameterDestination);
                      },
                      [&e, &wc, this](auto&&) {
                         emitConnectionLoadedWidget2Notes(
                             e.from.mdId, e.from.eventId.widgetId, wc.col,
                             wc.row, e.from.eventId.eventId,
                             e.from.eventId.channelId, e.to.mdId,
                             e.to.voiceIdx);
                      },
                  },
                  e.to.endpoint);
           },
           [&e, this](const Note& note) {
              mpark::visit(
                  util::overload{
                      [&e, note, this](const EventDestination::Note& destNote) {
                         emitConnectionLoadedNotes2Notes(
                             e.from.mdId, e.from.eventId.widgetId, note.number,
                             e.from.eventId.eventId, e.from.eventId.channelId,
                             e.to.mdId, e.to.voiceIdx);
                      },
                      [&e, note,
                       this](const EventDestination::ParameterBase& param) {
                         emitConnectionLoadedNotes2Parameter(
                             e.from.mdId, e.from.eventId.widgetId, note.number,
                             e.from.eventId.eventId, e.from.eventId.channelId,
                             e.to.mdId, e.to.voiceIdx, param.id,
                             param.parameterDestination);
                      },
                      [&e, note, this](auto&&) {
                         emitConnectionLoadedNotes2Notes(
                             e.from.mdId, e.from.eventId.widgetId, note.number,
                             e.from.eventId.eventId, e.from.eventId.channelId,
                             e.to.mdId, e.to.voiceIdx);
                      },
                  },
                  e.to.endpoint);
           },
           [](auto&&) {}},
       e.from.eventId.widgetCoord);
}

void EventRoutes::emitEntryGotDisabled(const MapEntry& e)
{
   mpark::visit(util::overload{
                    [&e, this](const WidgetCoord& wc) {
                       emitConnectionUnloadedWidget(
                           e.from.mdId, e.from.eventId.widgetId, wc.col, wc.row,
                           e.from.eventId.eventId, e.from.eventId.channelId);
                    },
                    [&e, this](const Note& note) {
                       emitConnectionUnloadedNotes(
                           e.from.mdId, e.from.eventId.widgetId, note.number,
                           e.from.eventId.eventId, e.from.eventId.channelId);
                    },
                    [](auto&&) {}},
                e.from.eventId.widgetCoord);
}

void EventRoutes::musicDeviceAppeared(const MusicDeviceId& mdId)
{
   MusicDeviceId _mdID = mdId;
   _mdID.portName      = MusicDeviceId::ANY_PORT;
   for (const auto& e : m_data)
   {
      if (e.from.mdId == _mdID || e.to.mdId == _mdID)
      {
         emitEntry(e);
      }
   }
}

void EventRoutes::musicDeviceDisappeared(const MusicDeviceId& mdId)
{
   for (const auto& e : m_data)
   {
      if (e.from.mdId == mdId || e.to.mdId == mdId)
      {
         emitEntryGotDisabled(e);
      }
   }
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
       controllerID, EventId{widgetIdx, WidgetCoord{widgetCoordY, widgetCoordX},
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
       controllerID, EventId{widgetIdx, WidgetCoord{widgetCoordY, widgetCoordX},
                             eventIdx, channelIdx}};
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   insert(from, to);
}

void EventRoutes::insert(const EventIdExt from, const EventDestinationL& to)
{
   auto it = std::find_if(m_data.begin(), m_data.end(),
                          [&from, &to](const MapEntry& e) {
                             return e.from == from && e.to == to;
                          });
   if (it != m_data.end())
   {
      return;
   }
   auto it2 =
       std::find_if(m_data.begin(), m_data.end(),
                    [&from](const MapEntry& e) { return e.from == from; });
   if (it2 != m_data.end())
   {
      m_data.erase(it2);
   }
   m_data.push_back(MapEntry{from, to});
   m_settings.save(CONFIG_SECTION, m_data);
}