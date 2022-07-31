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
      mutable int value;
   };
   struct ParameterBase
   {
      int id;
      ParameterDestination parameterDestination{ParameterDestination::Parameter};
   };

   struct Parameter : public ParameterBase
   {
      bool upwards{true};
      bool isList{false};
      int resolution {128};
      float zeroVal {0};
      mutable int storedIncrements{0};
      mutable std::optional<float> valueAtPress{0};
   };
   using Endpoint = mpark::variant<mpark::monostate, Note, Parameter,
                                   InternalFunctionality>;
   util::Identifiable::UUID uuid;
   int voiceIdx;
   Endpoint endpoint;
};

}   // namespace base::musicDevice::controller

#include "ControllerEventDestination.inl"
#include "ControllerEventDestinationMeta.h"

#endif
