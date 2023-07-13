#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H

#include "Identifiable.h"
#include "ParameterAttr.h"

namespace base::musicDevice::sound
{
struct ParameterCoordinate
{
   util::Identifiable::UUID uuid;
   int voiceIdx{0};
   int parameterIdx{0};
   ParameterAttr parameterAttr{ParameterAttr::Commanded};
   auto operator<=>(const ParameterCoordinate&) const = default;
};

}   // namespace base::musicDevice::sound

#endif
