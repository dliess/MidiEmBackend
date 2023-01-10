#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_META_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_LOADER_META_H

#include "Meta.h"


namespace base::eventRouter::loader{
#include "JsonCastNamespaceFix.h"
}

namespace meta
{

template<>
inline auto registerMembers<base::eventRouter::loader::EventIdExt>()
{
   return members(
      member("mdId", &base::eventRouter::loader::EventIdExt::mdId),
      member("eventId", &base::eventRouter::loader::EventIdExt::eventId));
}

template<>
inline auto registerMembers<base::eventRouter::EventDestination::Note>()
{
   return members(
      member("pitch", &base::eventRouter::EventDestination::Note::pitch));
}

template<>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::Note>(int i) noexcept
{
   return "Note";
}

template<>
inline auto registerMembers<base::eventRouter::EventDestination::ParameterBase>()
{
   return members(
      member("id", &base::eventRouter::EventDestination::ParameterBase::id),
      member("parameterDestination", &base::eventRouter::EventDestination::ParameterBase::parameterDestination));
}

template<>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::ParameterBase>(int i) noexcept
{
   return "Parameter";
}
/*
template<>
inline auto registerMembers<base::eventRouter::loader::EventDestinationL>()
{
   return members(
      member("mdId", &base::eventRouter::loader::EventDestinationL::mdId),
      member("voiceIdx", &base::eventRouter::loader::EventDestinationL::voiceIdx),
      member("controlType", &base::eventRouter::loader::EventDestinationL::controlType));
}
*/
template<>
inline auto registerMembers<base::eventRouter::loader::EventDestinationL>()
{
   return members(
      member("mdId", &base::eventRouter::loader::EventDestinationL::mdId),
      member("voiceIdx", &base::eventRouter::loader::EventDestinationL::voiceIdx));
}

template<>
inline auto registerMembers<base::eventRouter::loader::EventRoutes::MapEntry>()
{
   return members(
      member("from", &base::eventRouter::loader::EventRoutes::MapEntry::from),
      member("to", &base::eventRouter::loader::EventRoutes::MapEntry::to));
}

} // namespace meta

#endif