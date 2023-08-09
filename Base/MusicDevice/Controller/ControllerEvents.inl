#ifndef CONTROLLER_EVENTS_INL
#define CONTROLLER_EVENTS_INL

#include <functional>   // std::hash

#include "ControllerEvents.h"

namespace base::musicDevice::controller
{
inline bool operator==(const WidgetCoord& lhs, const WidgetCoord& rhs)
{
   return lhs.row == rhs.row && lhs.col == rhs.col;
}

inline bool operator<(const WidgetCoord& lhs, const WidgetCoord& rhs)
{
   if(lhs.row != rhs.row) return lhs.row < rhs.row;
   if(lhs.col != rhs.col) return lhs.col < rhs.col;
   return false;
}

inline bool operator==(const Note& lhs, const Note& rhs)
{
   return lhs.number == rhs.number;
}

inline bool operator<(const Note& lhs, const Note& rhs)
{
   return lhs.number < rhs.number;
}

inline bool operator==(const EventId& lhs, const EventId& rhs)
{
   return lhs.widgetId == rhs.widgetId && lhs.widgetCoord == rhs.widgetCoord &&
          lhs.eventId == rhs.eventId && lhs.channelId == rhs.channelId;
}

inline bool operator<(const EventId& lhs, const EventId& rhs)
{
   if(!(lhs.widgetId == rhs.widgetId)) return lhs.widgetId < rhs.widgetId;
   if(!(lhs.widgetCoord == rhs.widgetCoord)) return lhs.widgetCoord < rhs.widgetCoord;
   if(!(lhs.eventId == rhs.eventId)) return lhs.eventId < rhs.eventId;
   if(!(lhs.channelId == rhs.channelId)) return lhs.channelId < rhs.channelId;
   return false;
}

inline bool operator==(const PressReleaseType& lhs, const PressReleaseType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const ContinousValueType& lhs,
                       const ContinousValueType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const IncrementType& lhs, const IncrementType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const RelativeValueType& lhs,
                       const RelativeValueType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(Event const& lhs, Event const& rhs)
{
   return lhs.id == rhs.id && lhs.value == rhs.value;
}
/*
inline bool operator==(EventIdExt const& lhs, EventIdExt const& rhs)
{
   return lhs.uuid == rhs.uuid && lhs.eventId == rhs.eventId;
}
*/

}   // namespace base::musicDevice::controller

namespace std
{
template <> struct hash<base::musicDevice::controller::EventId::Coord>
{
   std::size_t operator()(
       const base::musicDevice::controller::EventId::Coord& wCoord) const
   {
      return (dl::visit(
          dl::overload{
              [](const base::musicDevice::controller::WidgetCoord& wCoord)
                  -> size_t {
                 return std::hash<int>{}(wCoord.row) ^
                        (std::hash<int>{}(wCoord.col) << 8);
              },
              [](const base::musicDevice::controller::Note& note) -> size_t {
                 return std::hash<int>{}(note.number);
              },
              [](auto&&) -> size_t { return std::hash<int>{}(0); }},
          wCoord) << (wCoord.index() * 8));
   }
};

template <> struct hash<base::musicDevice::controller::EventId>
{
   std::size_t operator()(
       const base::musicDevice::controller::EventId& eventId) const
   {
      return std::hash<int>{}(eventId.widgetId) ^
             std::hash<base::musicDevice::controller::EventId::Coord>{}(
                 eventId.widgetCoord) ^
             std::hash<int>{}(eventId.eventId) ^
             std::hash<int>{}(eventId.channelId);
   }
};

template <> struct hash<base::musicDevice::controller::EventIdExt>
{
   std::size_t operator()(
       const base::musicDevice::controller::EventIdExt& event) const
   {
      return util::IdentifiableHash{}(event.uuid) ^
             std::hash<base::musicDevice::controller::EventId>{}(event.eventId);
   }
};

}   // namespace std
#endif