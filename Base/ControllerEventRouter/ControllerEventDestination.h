#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H

#include <mpark/variant.hpp>
#include <optional>

#include "EnumReflect.h"
#include "Identifiable.h"
#include "MusicDeviceId.h"

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
      auto operator<=>(const DrumKit&) const = default;
   };
   struct Melodic
   {
      util::Identifiable::UUID uuid;
      int componentIdx;
      auto operator<=>(const Melodic&) const = default;
   };
   struct MusicDevice
   {
      musicDevice::MusicDeviceId mdid;
      int voiceIdx;
      auto operator<=>(const MusicDevice&) const = default;
   };
   using Endpoint = mpark::variant<DrumKit, Melodic, MusicDevice>;

   // ControlType
   struct Note
   {
      static constexpr int DefaultPitch = 64;
      mutable int pitch {DefaultPitch};
      auto operator<=>(const Note&) const = default;
   };
   struct Parameter
   {
      int id;
      ParameterDestination parameterDestination{
          ParameterDestination::Parameter};
      struct DescriptionCache {
         bool isList{false};
         int resolution{128};
         float zeroVal{0};
         bool upwards{true};
      } descriptionCache;
   };
   using ControlType = mpark::variant<Note, Parameter>;

   Endpoint endpoint;
   ControlType controlType;
};

}   // namespace base::musicDevice::controller

#include "ControllerEventDestinationHash.h"
#include "ControllerEventDestination.inl"
#include "ControllerEventDestinationMeta.h"

#endif
