#include <gtest/gtest.h>

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>


#include "MusicDeviceDescription.h"
namespace fs = std::filesystem;


TEST(MusicDeviceDescriptionTest, serializeMidiMsgIds)
{
   midi::MidiMessageId obj;
   nlohmann::json j(obj);
   decltype(obj) objRet; 
   j.get_to(objRet);
}

TEST(MusicDeviceDescriptionTest, serializeHubSection)
{
   base::musicDevice::description::hub::Section obj;
   nlohmann::json j(obj);
   decltype(obj) objRet; 
   j.get_to(objRet);
}

TEST(MusicDeviceDescriptionTest, serializeControllerSection)
{
   base::musicDevice::description::controller::Section obj;
   nlohmann::json j(obj);
   decltype(obj) objRet; 
   j.get_to(objRet);
}


TEST(MusicDeviceDescriptionTest, serializeSoundSection)
{
   base::musicDevice::description::sound::Section obj;
   nlohmann::json j(obj);
   decltype(obj) objRet;
   j.get_to(objRet);
}


TEST(MusicDeviceDescriptionTest, serializeDescription)
{
   base::musicDevice::description::Description devDescr;
   nlohmann::json j(devDescr);
   base::musicDevice::description::Description devDescrRet;
   j.get_to(devDescrRet);
}

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