#include "DeviceDescriptionLoader.h"
#include "MusicDeviceId.h"
#include "json.hpp"

#include <fstream>

using namespace base::musicDevice::description;

std::shared_ptr<Description> loadDescription(const std::string& configDir, const std::string& deviceName)
{
   const std::string devFilePath = fmt::format(
       "{}/MidiConfigs/Devices/{}/Config.json", configDir, deviceName);
   std::ifstream devFile(devFilePath);
   if (devFile.fail())
   {
      // So we assume its a midi interface
      auto pDescr = std::make_shared<Description>();
      pDescr->hubSection.emplace(hub::Section{true, true});
      return std::move(pDescr);
   }

   nlohmann::json jDev;
   try
   {
      devFile >> jDev;
   }
   catch (std::exception &e)
   {
      throw std::runtime_error(
          fmt::format("Parsing ill-formed json file '{}' failed, reason: {}",
                      devFilePath, e.what()));
   }
   auto pDescr = std::make_shared<Description>(jDev.get<Description>());
   // pDescr->relDevicePath = deviceName;
   return std::move(pDescr);
}