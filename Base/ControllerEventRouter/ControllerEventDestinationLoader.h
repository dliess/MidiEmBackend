#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_LOADER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_LOADER_H

#include <mpark/variant.hpp>
#include "ControllerEventDestination.h"
#include "MusicDeviceId.h"
#include "ControllerEvents.h"

namespace base::eventRouter::loader
{
struct EventIdExt
{
   musicDevice::MusicDeviceId mdId;
   musicDevice::controller::EventId eventId;
};

inline bool operator==(const EventIdExt& lhs, const EventIdExt& rhs)
{
   return lhs.mdId == rhs.mdId && lhs.eventId == rhs.eventId;
}

struct EventDestinationL
{
   musicDevice::MusicDeviceId mdId;
   int voiceIdx;
   using ControlType = mpark::variant<mpark::monostate, EventDestination::Note,
                                      EventDestination::Parameter>;
   ControlType controlType;
};

inline bool operator==(const EventDestinationL& lhs,
                       const EventDestinationL& rhs)
{
   return lhs.mdId == rhs.mdId && lhs.voiceIdx == rhs.voiceIdx &&
          lhs.controlType == rhs.controlType;
}

}   // namespace base::eventRouter::loader

#endif
