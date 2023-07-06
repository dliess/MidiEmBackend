#ifndef CONTROLLER_SECTION_META_H
#define CONTROLLER_SECTION_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace meta
{

template <>
inline auto registerMembers<base::musicDevice::description::controller::MidiSysexMsg>()
{
   return members(
      member("sysexDescriptors", &base::musicDevice::description::controller::MidiSysexMsg::sysexDescriptors)
   );
}


template <>
inline auto registerMembers<base::musicDevice::description::controller::WidgetDimension>()
{
   return members(
      member("numRows", &base::musicDevice::description::controller::WidgetDimension::numRows),
      member("numColumns", &base::musicDevice::description::controller::WidgetDimension::numColumns)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventIncremental>(int i) noexcept
{
   return "Incremental";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventIncremental>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventIncremental::name),
      member("global", &base::musicDevice::description::controller::EventIncremental::global),
      member("resolution", &base::musicDevice::description::controller::EventIncremental::resolution),
      member("source", &base::musicDevice::description::controller::EventIncremental::source)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventPressRelease>(int i) noexcept
{
   return "PressRelease";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventPressRelease>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventPressRelease::name),
      member("independent", &base::musicDevice::description::controller::EventPressRelease::independent),
      member("hasPressVelocity", &base::musicDevice::description::controller::EventPressRelease::hasPressVelocity),
      member("hasReleaseVelocity", &base::musicDevice::description::controller::EventPressRelease::hasReleaseVelocity),
      member("global", &base::musicDevice::description::controller::EventPressRelease::global),
      member("sourceHasInvertedLogic", &base::musicDevice::description::controller::EventPressRelease::sourceHasInvertedLogic),
      member("pressSource", &base::musicDevice::description::controller::EventPressRelease::pressSource),
      member("releaseSource", &base::musicDevice::description::controller::EventPressRelease::releaseSource)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventContinousValue>(int i) noexcept
{
   return "ContinousValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventContinousValue>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventContinousValue::name),
      member("global", &base::musicDevice::description::controller::EventContinousValue::global),
      member("startValueCanJump", &base::musicDevice::description::controller::EventContinousValue::startValueCanJump),
      member("source", &base::musicDevice::description::controller::EventContinousValue::source)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventRelativeValue>(int i) noexcept
{
   return "RelativeValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventRelativeValue>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventRelativeValue::name),
      member("bidirectional", &base::musicDevice::description::controller::EventRelativeValue::bidirectional),
      member("springsToZeroAtRelease", &base::musicDevice::description::controller::EventRelativeValue::springsToZeroAtRelease),
      member("fittingSemitones", &base::musicDevice::description::controller::EventRelativeValue::fittingSemitones),
      member("global", &base::musicDevice::description::controller::EventRelativeValue::global),
      member("source", &base::musicDevice::description::controller::EventRelativeValue::source)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::MidiSysexMsg>(int i) noexcept
{
   return "MidiSysexMsg";
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventDerivedContinousValue>(int i) noexcept
{
   return "DerivedRelativeValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventDerivedContinousValue>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventDerivedContinousValue::name),
      member("sourceEventIdx", &base::musicDevice::description::controller::EventDerivedContinousValue::sourceEventIdx)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventDerivedRelativeValue>(int i) noexcept
{
   return "DerivedRelativeValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventDerivedRelativeValue>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventDerivedRelativeValue::name),
      member("sourceEventIdx", &base::musicDevice::description::controller::EventDerivedRelativeValue::sourceEventIdx)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventDerivedIncremental>(int i) noexcept
{
   return "DerivedRelativeValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventDerivedIncremental>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventDerivedIncremental::name),
      member("sourceEventIdx", &base::musicDevice::description::controller::EventDerivedIncremental::sourceEventIdx)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::Widget>()
{
   return members(
      member("name", &base::musicDevice::description::controller::Widget::name),
      member("dimension", &base::musicDevice::description::controller::Widget::dimension),
      member("mpe", &base::musicDevice::description::controller::Widget::mpe),
      member("events", &base::musicDevice::description::controller::Widget::events)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::Section>()
{
   return members(
      member("widgets", &base::musicDevice::description::controller::Section::widgets),
      member("initMsg", &base::musicDevice::description::controller::Section::initMsg)
   );
}

} // namespace meta


#endif