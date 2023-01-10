#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_INL
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_INL

#include "ControllerEventDestination.h"

namespace base::eventRouter
{

inline bool operator==(const EventDestination::Note& lhs,
                       const EventDestination::Note& rhs)
{
   return lhs.pitch == rhs.pitch;
}

inline bool operator==(const EventDestination::Parameter& lhs,
                       const EventDestination::Parameter& rhs)
{
   return lhs.id == rhs.id && 
          lhs.parameterDestination == rhs.parameterDestination;
}

}

#endif