#ifndef CONTROLLER_EVENTS_META_H
#define CONTROLLER_EVENTS_META_H

#include "JsonCast.h"
#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::musicDevice::controller::WidgetCoord>()
{
   return members(member("row", &base::musicDevice::controller::WidgetCoord::row),
                  member("col", &base::musicDevice::controller::WidgetCoord::col));
}

template<>
inline auto registerMembers<base::musicDevice::controller::Note>()
{
   return members(member("number", &base::musicDevice::controller::Note::number));
}

template<>
inline auto registerMembers<base::musicDevice::controller::EventId>()
{
   return members(member("widgetId", &base::musicDevice::controller::EventId::widgetId),
                  member("widgetCoord", &base::musicDevice::controller::EventId::widgetCoord),
                  member("eventId", &base::musicDevice::controller::EventId::eventId),
                  member("channelId", &base::musicDevice::controller::EventId::channelId));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::EventId>(int i) noexcept
{
   return "EventId";
}

template<>
inline auto registerMembers<base::musicDevice::controller::PressReleaseType>()
{
   return members(member("value", &base::musicDevice::controller::PressReleaseType::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::PressReleaseType>(int i) noexcept
{
   return "PressReleaseType";
}

template<>
inline auto registerMembers<base::musicDevice::controller::ContinousValueType>()
{
   return members(member("value", &base::musicDevice::controller::ContinousValueType::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::ContinousValueType>(int i) noexcept
{
   return "ContinousValueType";
}

template<>
inline auto registerMembers<base::musicDevice::controller::IncrementType>()
{
   return members(member("value", &base::musicDevice::controller::IncrementType::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::IncrementType>(int i) noexcept
{
   return "IncrementType";
}

template<>
inline auto registerMembers<base::musicDevice::controller::RelativeValueType>()
{
   return members(member("value", &base::musicDevice::controller::RelativeValueType::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::RelativeValueType>(int i) noexcept
{
   return "RelativeValueType";
}

template<>
inline auto registerMembers<base::musicDevice::controller::RelativeUnlimitedValueType>()
{
   return members(member("value", &base::musicDevice::controller::RelativeUnlimitedValueType::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::RelativeUnlimitedValueType>(int i) noexcept
{
   return "RelativeUnlimitedValueType";
}

template<>
inline auto registerMembers<base::musicDevice::controller::Event>()
{
   return members(
      member("id", &base::musicDevice::controller::Event::id),
      member("value", &base::musicDevice::controller::Event::value)
   );
}

template<>
inline auto registerMembers<base::musicDevice::controller::EventIdExt>()
{
   return members(
      member("uuid", &base::musicDevice::controller::EventIdExt::uuid),
      member("eventId", &base::musicDevice::controller::EventIdExt::eventId)
   );
}

} // namespace meta

#endif