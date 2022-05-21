#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H

#include <mpark/variant.hpp>

#include "Identifiable.h"

namespace base::musicDevice::controller
{
struct EventDestination
{
   enum class InternalFunctionality
   {
      None = 0
   };
   struct Note
   {
      int value;
   };
   struct ParameterId
   {
      int id;
      int maxValue{1};
   };
   using Endpoint = mpark::variant<mpark::monostate, Note, ParameterId,
                                   InternalFunctionality>;
   util::Identifiable::UUID uuid;
   int voiceIdx;
   Endpoint endpoint;
};

}   // namespace base::musicDevice::controller

#endif
