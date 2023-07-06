#ifndef SOUND_SYSEX_DESCRIPTION_META_H
#define SOUND_SYSEX_DESCRIPTION_META_H

#include "JsonCast.h"
#include "Meta.h"
#include "SoundSysExDescription.h"

namespace meta
{
template <>
inline auto
registerMembers<base::musicDevice::description::sound::midisysex::VoiceIdx>()
{
   return members(
       member(
           "size",
           &base::musicDevice::description::sound::midisysex::VoiceIdx::size),
       member("mapping", &base::musicDevice::description::sound::midisysex::
                             VoiceIdx::mapping));
}

template <>
inline auto registerMembers<
    base::musicDevice::description::sound::midisysex::PatchNameStr>()
{
   return members(member(
       "size",
       &base::musicDevice::description::sound::midisysex::PatchNameStr::size));
}

template <>
inline auto registerMembers<
    base::musicDevice::description::sound::midisysex::PatchCategory>()
{
   return members(member(
       "size",
       &base::musicDevice::description::sound::midisysex::PatchCategory::size));
}

template <>
inline auto
registerMembers<base::musicDevice::description::sound::midisysex::PatchGenre>()
{
   return members(member(
       "size",
       &base::musicDevice::description::sound::midisysex::PatchGenre::size));
}

template <>
inline auto
registerMembers<base::musicDevice::description::sound::midisysex::Reserved>()
{
   return members(member(
       "size",
       &base::musicDevice::description::sound::midisysex::Reserved::size));
}

template <>
inline auto registerMembers<
    base::musicDevice::description::sound::midisysex::ParameterLowRes>()
{
   return members(
       member("component", &base::musicDevice::description::sound::midisysex::
                               ParameterLowRes::component),
       member("parameter", &base::musicDevice::description::sound::midisysex::
                               ParameterLowRes::parameter));
}

template <>
inline auto
getClassNameOrIndex<base::musicDevice::description::sound::midisysex::VoiceIdx>(
    int i) noexcept
{
   return "VoiceIdx";
}
template <>
inline auto getClassNameOrIndex<
    base::musicDevice::description::sound::midisysex::PatchNameStr>(
    int i) noexcept
{
   return "PatchNameStr";
}
template <>
inline auto getClassNameOrIndex<
    base::musicDevice::description::sound::midisysex::PatchCategory>(
    int i) noexcept
{
   return "PatchCategory";
}
template <>
inline auto getClassNameOrIndex<
    base::musicDevice::description::sound::midisysex::PatchGenre>(
    int i) noexcept
{
   return "PatchGenre";
}
template <>
inline auto
getClassNameOrIndex<base::musicDevice::description::sound::midisysex::Reserved>(
    int i) noexcept
{
   return "Reserved";
}
template <>
inline auto getClassNameOrIndex<
    base::musicDevice::description::sound::midisysex::ParameterLowRes>(
    int i) noexcept
{
   return "ParameterLowRes";
}
}   // namespace meta

#endif