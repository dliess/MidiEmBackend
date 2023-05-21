#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_H

#include <mpark/variant.hpp>
#include <optional>
#include <memory>
#include "EnumReflect.h"
#include "Identifiable.h"
#include "MusicDeviceId.h"
#include "Overload.h"
#include "ParameterPart.h"

namespace base::eventRouter
{

using ParameterPart = base::musicDevice::sound::ParameterPart;
struct EventDestination
{
   auto operator<=>(const EventDestination&) const = default;
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
      struct DescriptionCache 
      {
         bool isList{false};
         int resolution{128};
         float zeroVal{0};
         bool upwards{true};
      };
      struct ValueCache
      {
         int storedIncrements{0};
         std::optional<float> valueAtPress{0};
      };
      int id;
      ParameterPart parameterDestination{
          ParameterPart::Commanded};
      DescriptionCache descriptionCache;
      std::shared_ptr<ValueCache> valueCache;
   };
   using ControlType = mpark::variant<Note, Parameter>;

   Endpoint endpoint;
   ControlType controlType;
};

inline
void initRtCache(EventDestination& eventDestination)
{
   SWITCH(eventDestination.controlType)
      MFCASE_1(note) {},
      MFCASE_2(parameter)
      {
         if(!parameter.valueCache)
         {
            parameter.valueCache = 
               std::make_shared<EventDestination::Parameter::ValueCache>();
         }
      }
   END_SWITCH
}

}   // namespace base::eventRouter

#include "ControllerEventDestinationHash.h"
#include "ControllerEventDestination.inl"
#include "ControllerEventDestinationMeta.h"

#endif
