#ifndef CONTROLLER_EVENTS_H
#define CONTROLLER_EVENTS_H

#include <mpark/variant.hpp>
#include <string>
#include "Identifiable.h"
#include "Overload.h"

namespace base::musicDevice::controller
{
constexpr int INVALID_IDX = -1;

struct WidgetCoord
{
   int row{INVALID_IDX};
   int col{INVALID_IDX};
};

struct Note
{
   int number;
};
struct EventId
{
   int widgetId;
   using Coord = mpark::variant<mpark::monostate, WidgetCoord, Note>;
   Coord widgetCoord;
   int eventId;
   int channelId{0};
};

struct PressReleaseType
{
   float value; // -1.0 to 1.0
};
struct ContinousValueType
{
   float value; // 0.0 to 1.0
};
struct IncrementType
{
   int resolution;
   int value;
};
struct RelativeValueType
{
   float value;
};

struct RelativeUnlimitedValueType
{
   int value{0};
   int incrementsPerCentimeter{1};
};

using EventValue = mpark::variant<mpark::monostate, PressReleaseType,
                                  ContinousValueType, IncrementType, 
                                  RelativeValueType, RelativeUnlimitedValueType>;

struct Event
{
   EventId id;
   EventValue value;
};

template<typename IdType>
struct EventIdAndUuid
{
   IdType uuid;
   EventId eventId;
   auto operator<=>(const EventIdAndUuid<IdType>&) const = default;
};

using EventIdExt = EventIdAndUuid<util::Identifiable::UUID>;

} // namespace base::musicDevice::controller

#include "ControllerEvents.inl"
#include "ControllerEventsMeta.h"
#endif