#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H

#include "ParameterPart.h"
#include "Identifiable.h"

namespace base::musicDevice::sound
{

struct ParameterCoordinate
{
   util::Identifiable::UUID uuid;
   int voiceIdx{0};
   int parameterIdx{0};
   ParameterPart parameterPart{ParameterPart::Commanded};
};

}   // namespace base::musicDevice::sound
#endif
