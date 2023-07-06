#ifndef SOUND_SYSEX_DESCRIPTION_H
#define SOUND_SYSEX_DESCRIPTION_H

#include "SysExDescription.h"
#include <mpark/variant.hpp>

namespace base::musicDevice::description::sound::midisysex
{

struct VoiceIdx : public base::musicDevice::description::midisysex::FieldWithSize
{
   std::vector<int> mapping;
};
struct PatchNameStr : public base::musicDevice::description::midisysex::FieldWithSize
{
};
struct PatchCategory : public base::musicDevice::description::midisysex::FieldWithSize
{
};
struct PatchGenre : public base::musicDevice::description::midisysex::FieldWithSize
{
};
struct Reserved : public base::musicDevice::description::midisysex::FieldWithSize
{
};

struct ParameterLowRes : public base::musicDevice::description::midisysex::Field
{
   std::string component;
   std::string parameter;
};

using FieldDescr = mpark::variant<base::musicDevice::description::midisysex::Bytes, 
                                  VoiceIdx, PatchNameStr, PatchCategory,
                                  PatchGenre, Reserved, ParameterLowRes>;

}   // namespace base::musicDevice::description::sound::midisysex

#include "SoundSysExDescriptionMeta.h"

#endif