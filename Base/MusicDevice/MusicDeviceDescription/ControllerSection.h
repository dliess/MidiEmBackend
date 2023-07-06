#ifndef CONTROLLER_SECTION_H
#define CONTROLLER_SECTION_H

#include <mpark/variant.hpp>
#include <string>

#include "MidiMessageIds.h"
#include "MidiMessageIdsMeta.h"

#include "ControllerSysExDescription.h"


namespace base::musicDevice::description::controller
{

using SysExDescriptors = std::vector<midisysex::FieldDescr>;
struct MidiSysexMsg
{
   SysExDescriptors sysexDescriptors;
};

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
   std::optional<int> keytrackEvtIdx; // Dont Serialize, its a helper value
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
   bool springsToZeroAtRelease;
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
   std::optional<MidiSysexMsg> initMsg;
};

} // namespace base::musicDevice::description::controller

#include "ControllerSection.inl"
#include "ControllerSectionMeta.h"

#endif