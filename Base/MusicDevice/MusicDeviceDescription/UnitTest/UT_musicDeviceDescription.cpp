#include <gtest/gtest.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>


#include "MusicDeviceDescription.h"
namespace fs = std::filesystem;

TEST(MusicDeviceDescriptionTest, verifyJsons)
{
   const std::string devicesPath = fmt::format("{}/MidiConfigs/Devices", RESOURCE_ROOT_DIR);
   for (const auto& manufacturerPath : fs::directory_iterator(devicesPath))
   {
      for (const auto& devicePath : fs::directory_iterator(manufacturerPath))
      {
         const std::string jsonFileName = fmt::format("{}/Config.json", std::string(devicePath.path()));
         std::ifstream jsonFile(jsonFileName);
         ASSERT_FALSE(jsonFile.fail()) << jsonFileName;
         nlohmann::json jsonParsed;
         ASSERT_NO_THROW(jsonFile >> jsonParsed) << jsonFileName;
         base::musicDevice::description::Description devDescr;
         ASSERT_NO_THROW(devDescr = jsonParsed.get<base::musicDevice::description::Description>()) << jsonFileName;
         ASSERT_NO_THROW(devDescr.checkValidity());
      }
   }
}
