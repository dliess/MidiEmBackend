#ifndef CONTROLLER_SECTION_H
#define CONTROLLER_SECTION_H

#include <mpark/variant.hpp>
#include <string>

#include "MidiMessageIds.h"
#include "MidiMessageIdsMeta.h"

namespace base::musicDevice::description::controller
{
struct WidgetDimension
{
   int numRows;
   int numColumns;
};

struct EventIncremental
{
   std::string name;
   int resolution;
   std::vector<std::vector<midi::MidiMessageId>> source;
};

struct EventPressRelease
{
   std::string name;
   std::optional<bool> sourceHasInvertedLogic;
   std::vector<std::vector<midi::MidiMessageId>> pressSource;
   std::vector<std::vector<midi::MidiMessageId>> releaseSource;
};

struct EventContinousValue
{
   std::string name;
   std::vector<std::vector<midi::MidiMessageId>> source;
};

struct EventRelativeValue
{
   std::string name;
   std::vector<std::vector<midi::MidiMessageId>> source;
};

using Event = mpark::variant<
   EventIncremental, EventPressRelease,
   EventContinousValue, EventRelativeValue>;

struct Widget
{
   std::string name;
   WidgetDimension dimension;
   std::optional<bool> mpe;
   std::vector<Event> events;
};

struct Section
{
   std::vector<Widget> widgets;
};

} // namespace base::musicDevice::description::controller

#include "ControllerSectionMeta.h"

#endif