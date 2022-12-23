#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_META_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_META_H

#include "Meta.h"


namespace base::musicDevice::controller::loader{
#include "JsonCastNamespaceFix.h"
}

namespace meta
{

template<>
inline auto registerMembers<base::musicDevice::controller::loader::EventIdExt>()
{
   return members(
      member("mdId", &base::musicDevice::controller::loader::EventIdExt::mdId),
      member("eventId", &base::musicDevice::controller::loader::EventIdExt::eventId));
}

template<>
inline auto registerMembers<base::musicDevice::controller::EventDestination::Note>()
{
   return members(
      member("value", &base::musicDevice::controller::EventDestination::Note::value));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::EventDestination::Note>(int i) noexcept
{
   return "Note";
}

template<>
inline auto registerMembers<base::musicDevice::controller::EventDestination::ParameterBase>()
{
   return members(
      member("id", &base::musicDevice::controller::EventDestination::ParameterBase::id),
      member("parameterDestination", &base::musicDevice::controller::EventDestination::ParameterBase::parameterDestination));
}

template<>
inline auto getClassNameOrIndex<base::musicDevice::controller::EventDestination::ParameterBase>(int i) noexcept
{
   return "Parameter";
}

template<>
inline auto registerMembers<base::musicDevice::controller::loader::EventDestinationL>()
{
   return members(
      member("mdId", &base::musicDevice::controller::loader::EventDestinationL::mdId),
      member("voiceIdx", &base::musicDevice::controller::loader::EventDestinationL::voiceIdx),
      member("controlType", &base::musicDevice::controller::loader::EventDestinationL::controlType));
}

template<>
inline auto registerMembers<base::musicDevice::controller::loader::EventRoutes::MapEntry>()
{
   return members(
      member("from", &base::musicDevice::controller::loader::EventRoutes::MapEntry::from),
      member("to", &base::musicDevice::controller::loader::EventRoutes::MapEntry::to));
}

} // namespace meta

#endif