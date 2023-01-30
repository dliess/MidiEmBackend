#ifndef DEVICE_PRESETS_TYPES_H
#define DEVICE_PRESETS_TYPES_H

#include <string>
#include <optional>
#include <vector>

#include "MusicDeviceId.h"
#include "EnumReflect.h"
#include "ParameterData.h"

namespace base::musicDevice::sound::preset
{
/*
   Identifier of a preset:
      manufacturer+product + engineIdx + presetName
*/
struct Id
{
   MusicDeviceName musicDeviceName;
   int engineIdx;
   std::string presetName;
};

DECLARE_ENUM(Genre, uint, None, Classic, DBBreaks, House, Industrial, Jazz,
             RBHHop, RockPop, Techno, Dubstep);
DECLARE_ENUM(Category, uint, None, Arp, Bass, Bell, Classic, Drum, Keyboard,
             Lead, Movement, Pad, Poly, SFX, String, User, Voc);


struct Preset
{
   std::optional<int> slotOnDeviceIndex;
   Category category;
   Genre genre;
   std::vector<ParameterData> parameters;
};

}   // namespace base::musicDevice::sound::preset

#endif