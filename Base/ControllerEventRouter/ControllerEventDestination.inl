#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_INL
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_INL

#include "ControllerEventDestination.h"

namespace base::musicDevice::controller
{

inline bool operator==(const EventDestination::Note& lhs,
                       const EventDestination::Note& rhs)
{
   return lhs.value == rhs.value;
}

inline bool operator==(const EventDestination::ParameterBase& lhs,
                       const EventDestination::ParameterBase& rhs)
{
   return lhs.id == rhs.id && 
          lhs.parameterDestination == rhs.parameterDestination;
}

}

#endif