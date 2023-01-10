#include "ControllerEventRouterLoader.h"

#include "ControllerHandler.h"
#include "MusicDeviceContainer.h"

using namespace base::eventRouter;

const std::string loader::EventRoutes::CONFIG_SECTION = "ControllerEventRoutes";

loader::EventRoutes::EventRoutes() :
    m_settings("ControllerEventRoutes", "ControllerEventRoutes.json")
{
}

void loader::EventRoutes::loadFromFile()
{
   m_data = m_settings.load<decltype(m_data)>(CONFIG_SECTION);
   for (const auto& e : m_data) { emitEntry(e); }
}

void loader::EventRoutes::emitEntry(const MapEntry& e)
{
   mpark::visit(
       util::overload{
           [&e, this](const musicDevice::controller::WidgetCoord& wc) {
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
                  e.to.controlType);
           },
           [&e, this](const musicDevice::controller::Note& note) {
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
                  e.to.controlType);
           },
           [](auto&&) {}},
       e.from.eventId.widgetCoord);
}

void loader::EventRoutes::emitEntryGotDisabled(const MapEntry& e)
{
   mpark::visit(util::overload{
                    [&e, this](const musicDevice::controller::WidgetCoord& wc) {
                       emitConnectionUnloadedWidget(
                           e.from.mdId, e.from.eventId.widgetId, wc.col, wc.row,
                           e.from.eventId.eventId, e.from.eventId.channelId);
                    },
                    [&e, this](const musicDevice::controller::Note& note) {
                       emitConnectionUnloadedNotes(
                           e.from.mdId, e.from.eventId.widgetId, note.number,
                           e.from.eventId.eventId, e.from.eventId.channelId);
                    },
                    [](auto&&) {}},
                e.from.eventId.widgetCoord);
}

void loader::EventRoutes::musicDeviceAppeared(const musicDevice::MusicDeviceId& mdId)
{
   musicDevice::MusicDeviceId _mdID = mdId;
   _mdID.portName                   = musicDevice::MusicDeviceId::ANY_PORT;
   for (const auto& e : m_data)
   {
      if (e.from.mdId == _mdID || e.to.mdId == _mdID)
      {
         emitEntry(e);
      }
   }
}

void loader::EventRoutes::musicDeviceDisappeared(const musicDevice::MusicDeviceId& mdId)
{
   for (const auto& e : m_data)
   {
      if (e.from.mdId == mdId || e.to.mdId == mdId)
      {
         emitEntryGotDisabled(e);
      }
   }
}

void loader::EventRoutes::connectNotes2Notes(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx, int note,
    int eventIdx, int channelIdx, const musicDevice::MusicDeviceId& soundDevID,
    int voiceIdx)
{
   const EventIdExt from{controllerID,
                         musicDevice::controller::EventId{
                             widgetIdx, musicDevice::controller::Note{note},
                             eventIdx, channelIdx}};
   const EventDestinationL to{soundDevID, voiceIdx, mpark::monostate()};
   insert(from, to);
}

void loader::EventRoutes::connectNotes2Parameter(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx, int note,
    int eventIdx, int channelIdx, const musicDevice::MusicDeviceId& soundDevID,
    int voiceIdx, int parameterIdx, ParameterDestination paramFunc)
{
   const EventIdExt from{controllerID,
                         musicDevice::controller::EventId{
                             widgetIdx, musicDevice::controller::Note{note},
                             eventIdx, channelIdx}};
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   insert(from, to);
}

void loader::EventRoutes::connectWidget2Notes(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
    int widgetCoordX, int widgetCoordY, int eventIdx, int channelIdx,
    const musicDevice::MusicDeviceId& soundDevID, int voiceIdx)
{
   const EventIdExt from{controllerID, musicDevice::controller::EventId{
                                           widgetIdx,
                                           musicDevice::controller::WidgetCoord{
                                               widgetCoordY, widgetCoordX},
                                           eventIdx, channelIdx}};
   const EventDestinationL to{soundDevID, voiceIdx, mpark::monostate()};
   insert(from, to);
}

void loader::EventRoutes::connectWidget2Parameter(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
    int widgetCoordX, int widgetCoordY, int eventIdx, int channelIdx,
    const musicDevice::MusicDeviceId& soundDevID, int voiceIdx,
    int parameterIdx, ParameterDestination paramFunc)
{
   const EventIdExt from{controllerID, musicDevice::controller::EventId{
                                           widgetIdx,
                                           musicDevice::controller::WidgetCoord{
                                               widgetCoordY, widgetCoordX},
                                           eventIdx, channelIdx}};
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   insert(from, to);
}

void loader::EventRoutes::eraseConnectionForNotes(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx, int note,
    int eventIdx, int channelIdx)
{
   const EventIdExt from{
       controllerID, musicDevice::controller::EventId{widgetIdx, musicDevice::controller::Note{note},
                                                      eventIdx, channelIdx}};
   erase(from);
}

void loader::EventRoutes::eraseConnectionForWidget(
    const musicDevice::MusicDeviceId& controllerID, int widgetIdx,
    int widgetCoordX, int widgetCoordY, int eventIdx, int channelIdx)
{
   const EventIdExt from{controllerID,
                         musicDevice::controller::EventId{
                             widgetIdx, musicDevice::controller::WidgetCoord{widgetCoordY, widgetCoordX},
                             eventIdx, channelIdx}};
   erase(from);
}

void loader::EventRoutes::eraseConnectionsToDestinationNotes(
    const musicDevice::MusicDeviceId& soundDevID, int voiceIdx)
{
   std::vector<EventIdExt> toErase;
   const EventDestinationL to{soundDevID, voiceIdx, mpark::monostate()};
   for (auto& e : m_data)
   {
      if (e.to == to)
      {
         toErase.push_back(e.from);
      }
   }
   for (const auto& from : toErase) { erase(from); }
}

void loader::EventRoutes::eraseConnectionsToDestinationParameter(
    const musicDevice::MusicDeviceId& soundDevID, int voiceIdx,
    int parameterIdx, ParameterDestination paramFunc)
{
   std::vector<EventIdExt> toErase;
   const EventDestinationL to{
       soundDevID, voiceIdx,
       EventDestination::ParameterBase{parameterIdx, paramFunc}};
   for (auto& e : m_data)
   {
      if (e.to == to)
      {
         toErase.push_back(e.from);
      }
   }
   for (const auto& from : toErase) { erase(from); }
}

void loader::EventRoutes::insert(const EventIdExt& from, const EventDestinationL& to)
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
   const MapEntry mapEntry{from, to};
   m_data.push_back(mapEntry);
   emitEntry(mapEntry);
   m_settings.save(CONFIG_SECTION, m_data);
}

void loader::EventRoutes::erase(const EventIdExt& from)
{
   auto it =
       std::find_if(m_data.begin(), m_data.end(),
                    [&from](const MapEntry& e) { return e.from == from; });
   if (it != m_data.end())
   {
      emitEntryGotDisabled(*it);
      m_data.erase(it);
      m_settings.save(CONFIG_SECTION, m_data);
   }
}
