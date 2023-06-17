#ifndef MUSIC_DEVICE_DESCRIPTION_H
#define MUSIC_DEVICE_DESCRIPTION_H

#include <mpark/variant.hpp>
#include <optional>
#include <string>
#include <vector>

#include "ControllerSection.h"
#include "JsonCast.h"
#include "Meta.h"
#include "SoundSection.h"
#include "HubSection.h"

namespace base::musicDevice::description
{
struct Description
{
   std::string relDevicePath; // NOT REFFLECTED

   std::string version;
   std::string manufacturer;
   std::string productName;
   std::optional<sound::Section> soundSection;
   std::optional<controller::Section> controllerSection;
   std::optional<hub::Section> hubSection;
   std::optional<bool> reactsToTransportCommands;

   //----------------------------------------
   //------------- Methods ------------------
   //----------------------------------------

   void checkValidity() const;
   void initCaches() noexcept;
   void createAdditionalControllerEvents();
};

} // namespace base::musicDevice::description

#include "MusicDeviceDescriptionMeta.h"

#endif