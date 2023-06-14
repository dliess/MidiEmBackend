#ifndef CONTROLLER_SECTION_META_H
#define CONTROLLER_SECTION_META_H

#include "Meta.h"
#include "JsonCast.h"

namespace meta
{

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
      member("global", &base::musicDevice::description::controller::EventRelativeValue::global),
      member("source", &base::musicDevice::description::controller::EventRelativeValue::source)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::controller::EventRelativeUnlimitedValue>(int i) noexcept
{
   return "RelativeUnlimitedValue";
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::EventRelativeUnlimitedValue>()
{
   return members(
      member("name", &base::musicDevice::description::controller::EventRelativeUnlimitedValue::name),
      member("incrementsPerSemitoneDistance", &base::musicDevice::description::controller::EventRelativeUnlimitedValue::incrementsPerSemitoneDistance),
      member("global", &base::musicDevice::description::controller::EventRelativeUnlimitedValue::global),
      member("source", &base::musicDevice::description::controller::EventRelativeUnlimitedValue::source)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::Widget>()
{
   return members(
      member("name", &base::musicDevice::description::controller::Widget::name),
      member("dimension", &base::musicDevice::description::controller::Widget::dimension),
      member("mpe", &base::musicDevice::description::controller::Widget::mpe),
      member("isKeyboard", &base::musicDevice::description::controller::Widget::isKeyboard),
      member("events", &base::musicDevice::description::controller::Widget::events)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::controller::Section>()
{
   return members(
      member("widgets", &base::musicDevice::description::controller::Section::widgets)
   );
}

} // namespace meta


#endif