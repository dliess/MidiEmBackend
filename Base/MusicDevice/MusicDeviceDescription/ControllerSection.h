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

struct EventPressRelease
{
   std::string name;
   std::optional<bool> independent;
   std::optional<bool> hasPressVelocity;
   std::optional<bool> hasReleaseVelocity;
   std::optional<bool> global;
   std::optional<bool> sourceHasInvertedLogic;
   std::vector<std::vector<midi::MidiMessageId>> pressSource;
   std::vector<std::vector<midi::MidiMessageId>> releaseSource;
   std::optional<int> pressVelocityEvtIdx; // Dont Serialize, its a helper value
   std::optional<int> releaseVelocityEvtIdx; // Dont Serialize, its a helper value
};

struct TwinData
{
   int indepPressEvtIdx;
   int twinEvtIdx;
};

struct EventIncremental
{
   std::string name;
   std::optional<bool> global;
   int resolution;
   std::vector<std::vector<midi::MidiMessageId>> source;
   std::optional<TwinData> twin; // Dont Serialize, its a helper value
};

struct EventContinousValue
{
   std::string name;
   std::optional<bool> global;
   std::optional<bool> startValueCanJump;
   std::vector<std::vector<midi::MidiMessageId>> source;
   std::optional<TwinData> twin; // Dont Serialize, its a helper value
};

struct EventRelativeValue
{
   std::string name;
   bool bidirectional;
   size_t fittingSemitones;
   std::optional<bool> global;
   std::vector<std::vector<midi::MidiMessageId>> source;
   std::optional<TwinData> twin; // Dont Serialize, its a helper value
};

struct EventDerivedContinousValue
{
   std::string name;
   int sourceEventIdx; // Dont Serialize, its a helper value
};
struct EventDerivedRelativeValue
{
   std::string name;
   int sourceEventIdx; // Dont Serialize, its a helper value
};
struct EventDerivedIncremental
{
   std::string name;
   int sourceEventIdx; // Dont Serialize, its a helper value
};

using Event = mpark::variant<
   EventIncremental, EventPressRelease,
   EventContinousValue,
   EventDerivedContinousValue,
   EventRelativeValue,
   EventDerivedIncremental,
   EventDerivedRelativeValue>;

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