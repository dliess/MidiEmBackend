#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_COORDINATE_H

#include "Identifiable.h"
#include "ParameterPart.h"

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

inline bool operator==(
    const base::musicDevice::sound::ParameterCoordinate& lhs,
    const base::musicDevice::sound::ParameterCoordinate& rhs) noexcept
{
   return (lhs.uuid == rhs.uuid) && (lhs.voiceIdx == rhs.voiceIdx) &&
          (lhs.parameterIdx == rhs.parameterIdx) &&
          (lhs.parameterPart == rhs.parameterPart);
}

#endif
