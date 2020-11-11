#ifndef CONTROLLER_EVENTS_H
#define CONTROLLER_EVENTS_H

#include <mpark/variant.hpp>
#include <string>

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
   int value;
};
struct RelativeValueType
{
   float value;
};

using EventValue = mpark::variant<mpark::monostate, PressReleaseType,
                                  ContinousValueType, IncrementType, RelativeValueType>;

struct Event
{
   EventId id;
   EventValue value;
};

} // namespace base::musicDevice::controller

#include "ControllerEventsMeta.h"

#endif