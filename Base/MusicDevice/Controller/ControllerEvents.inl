#ifndef CONTROLLER_EVENTS_INL
#define CONTROLLER_EVENTS_INL

#include "ControllerEvents.h"
#include <functional> // std::hash

namespace base::musicDevice::controller
{

inline bool operator==(const WidgetCoord& lhs, const WidgetCoord& rhs)
{
   return lhs.row == rhs.row && 
          lhs.col == rhs.col;
}

inline bool operator==(const Note& lhs, const Note& rhs)
{
   return lhs.number == rhs.number;
}

inline bool operator==(const EventId& lhs, const EventId& rhs)
{
   return lhs.widgetId == rhs.widgetId &&
          lhs.widgetCoord == rhs.widgetCoord &&
          lhs.eventId == rhs.eventId;
}

inline bool operator==(const PressReleaseType& lhs, const PressReleaseType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const ContinousValueType& lhs, const ContinousValueType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const IncrementType& lhs, const IncrementType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const RelativeValueType& lhs, const RelativeValueType& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(Event const& lhs, Event const& rhs)
{
   return lhs.id == rhs.id &&
          lhs.value == rhs.value;
}

inline bool operator==(EventExt const& lhs, EventExt const& rhs)
{
   return lhs.uuid == rhs.uuid &&
          lhs.event == rhs.event;
}

} // namespace base::musicDevice::controller

namespace std {
    template<> struct hash<base::musicDevice::controller::Event> {
        std::size_t operator()(const base::musicDevice::controller::Event &event) const {
           //TODO
            return 1;
        }  
    };

    template<> struct hash<base::musicDevice::controller::EventExt> {
        std::size_t operator()(const base::musicDevice::controller::EventExt &event) const {
           //TODO
            return 1;
        }  
    };

}
#endif