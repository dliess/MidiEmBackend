#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H

#include <mpark/variant.hpp>
#include <optional>

#include "EnumReflect.h"
#include "Identifiable.h"

namespace base::musicDevice::controller
{
DECLARE_ENUM(ParameterDestination, uint, Parameter, LFOWaveform, LFOAmplitude,
             LFOFrequency, LFOMultiplier);
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
   struct Parameter
   {
      int id;
      bool upwards{true};
      ParameterDestination parameterDestination{ParameterDestination::Parameter};
      bool isList{false};
      size_t resolution {128};
      mutable std::optional<float> valueAtPress{0};
   };
   using Endpoint = mpark::variant<mpark::monostate, Note, Parameter,
                                   InternalFunctionality>;
   util::Identifiable::UUID uuid;
   int voiceIdx;
   Endpoint endpoint;
};

}   // namespace base::musicDevice::controller

#endif
