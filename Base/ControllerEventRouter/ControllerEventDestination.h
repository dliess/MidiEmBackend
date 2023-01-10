#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H

#include <mpark/variant.hpp>
#include <optional>

#include "EnumReflect.h"
#include "Identifiable.h"

namespace base::eventRouter
{
DECLARE_ENUM(ParameterDestination, uint, Parameter, LFOWaveform, LFOAmplitude,
             LFOFrequency, LFOMultiplier);
struct EventDestination
{
   // Endpoint
   struct DrumKit
   {
      static constexpr int NOT_SET = -2;
      util::Identifiable::UUID uuid;
      int voiceIdx;
      int componentIdx{NOT_SET};
      // auto operator<=>(const DrumKit&) const = default;
   };
   struct Melodic
   {
      util::Identifiable::UUID uuid;
      int componentIdx;
      // auto operator<=>(const Melodic&) const = default;
   };
   struct MusicDevice
   {
      util::Identifiable::UUID uuid;
      int voiceIdx;
      // auto operator<=>(const MusicDevice&) const = default;
   };
   using Endpoint = mpark::variant<DrumKit, Melodic, MusicDevice>;

   // ControlType
   struct Note
   {
      static constexpr int DefaultPitch = 64;
      mutable int pitch {DefaultPitch};
   };
   struct ParameterBase
   {
      int id;
      ParameterDestination parameterDestination{
          ParameterDestination::Parameter};
   };
   struct Parameter : public ParameterBase
   {
      bool upwards{true};
      bool isList{false};
      int resolution{128};
      float zeroVal{0};
      mutable int storedIncrements{0};
      mutable std::optional<float> valueAtPress{0};
   };
   using ControlType = mpark::variant<Note, Parameter>;

   Endpoint endpoint;
   ControlType controlType;
};

}   // namespace base::musicDevice::controller

#include "ControllerEventDestination.inl"
#include "ControllerEventDestinationMeta.h"

#endif
