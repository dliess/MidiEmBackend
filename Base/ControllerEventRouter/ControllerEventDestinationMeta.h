#ifndef BASE_EVENT_ROUTE_EVENT_DESTINATION_META_H
#define BASE_EVENT_ROUTE_EVENT_DESTINATION_META_H

#include "JsonCast.h"
#include "Meta.h"

namespace base::eventRouter
{
inline void to_json(
    nlohmann::json& j, const ParameterDestination& obj)
{
   j = ~obj;
}

inline void from_json(
    const nlohmann::json& j, ParameterDestination& obj)
{
   obj = create_ParameterDestination(j.get<std::string>());
}

}   // namespace base::eventRouter


namespace meta
{
template <>
inline auto registerMembers<base::eventRouter::EventDestination::DrumKit>()
{
   return members(
       member("uuid", &base::eventRouter::EventDestination::DrumKit::uuid),
       member("voiceIdx",
              &base::eventRouter::EventDestination::DrumKit::voiceIdx),
       member("componentIdx",
              &base::eventRouter::EventDestination::DrumKit::componentIdx));
}
template <>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::DrumKit>(
    int i) noexcept
{
   return "DrumKit";
}

template <>
inline auto registerMembers<base::eventRouter::EventDestination::Melodic>()
{
   return members(
       member("uuid", &base::eventRouter::EventDestination::Melodic::uuid),
       member("componentIdx",
              &base::eventRouter::EventDestination::Melodic::componentIdx));
}
template <>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::Melodic>(
    int i) noexcept
{
   return "Melodic";
}

template <>
inline auto registerMembers<base::eventRouter::EventDestination::MusicDevice>()
{
   return members(
       member("mdid", &base::eventRouter::EventDestination::MusicDevice::mdid),
       member("voiceIdx",
              &base::eventRouter::EventDestination::MusicDevice::voiceIdx));
}
template <>
inline auto
getClassNameOrIndex<base::eventRouter::EventDestination::MusicDevice>(
    int i) noexcept
{
   return "MusicDevice";
}

template <>
inline auto registerMembers<base::eventRouter::EventDestination::Note>()
{
   return members(
       member("pitch", &base::eventRouter::EventDestination::Note::pitch));
}
template <>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::Note>(
    int i) noexcept
{
   return "Note";
}

template <>
inline auto registerMembers<base::eventRouter::EventDestination::Parameter>()
{
   return members(
       member("id", &base::eventRouter::EventDestination::Parameter::id),
       member("parameterDestination", &base::eventRouter::EventDestination::
                                          Parameter::parameterDestination));
}
template <>
inline auto getClassNameOrIndex<base::eventRouter::EventDestination::Parameter>(
    int i) noexcept
{
   return "Parameter";
}

template <> inline auto registerMembers<base::eventRouter::EventDestination>()
{
   return members(
       member("endpoint", &base::eventRouter::EventDestination::endpoint),
       member("controlType",
              &base::eventRouter::EventDestination::controlType));
}

}   // namespace meta

#endif
