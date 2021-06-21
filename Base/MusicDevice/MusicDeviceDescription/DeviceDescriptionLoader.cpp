#include "DeviceDescriptionLoader.h"

#include <fmt/format.h>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <loguru.hpp>

#include "MusicDeviceId.h"

using namespace base::musicDevice;

description::Loader::Loader(const std::string &configDir) :
    m_configDir(configDir.empty() ? "." : configDir)
{
   const std::string mapFileName(
       fmt::format("{}/MidiConfigs/usbMidiName2device.json", m_configDir));
   const std::string deviceChainsFileName(
       fmt::format("{}/MidiConfigs/midiDeviceChains.json", m_configDir));
   std::ifstream mapFile(mapFileName);
   std::ifstream deviceChainsFile(deviceChainsFileName);
   if (mapFile.fail())
   {
      throw std::runtime_error(
          fmt::format("could not find file '{}'", mapFileName));
   }
   if (deviceChainsFile.fail())
   {
      LOG_F(INFO,
            "There is no custom midi interface connection config file '{}'",
            deviceChainsFileName);
   }
   else
   {
      try
      {
         json jDeviceChains;
         deviceChainsFile >> jDeviceChains;
         m_deviceChains = jDeviceChains.get<DeviceChains>();
      }
      catch (json::type_error &e)
      {
         LOG_F(ERROR, "ERROR at parsing ill formed '{}' reason: {}",
               deviceChainsFileName, e.what());
      }
      catch (...)
      {
         LOG_F(ERROR, "ERROR at parsing ill formed '{}'", deviceChainsFileName);
      }
   }

   try
   {
      mapFile >> m_jUsbMidiName2deviceMap;
   }
   catch (...)
   {
      LOG_F(ERROR, "ERROR at parsing ill formed '{}'", mapFileName);
   }
}

std::pair<description::Loader::ResultType, std::string>
description::Loader::getMatchType(const std::string &deviceName) const noexcept
{
   const auto iter = m_jUsbMidiName2deviceMap.find(deviceName);
   if (m_jUsbMidiName2deviceMap.end() == iter)
   {
      return std::make_pair(ResultType::NotFound, deviceName);
   }
   if (iter->get<std::string>() == "--UNUSED--")
   {
      return std::make_pair(ResultType::MarkedUnused, "");
   }
   else
   {
      return std::make_pair(ResultType::MusicDevice, iter->get<std::string>());
   }
}

std::shared_ptr<description::Description> description::Loader::load(
    const std::string &deviceName) const
{
   const std::string devFilePath = fmt::format(
       "{}/MidiConfigs/Devices/{}/Config.json", m_configDir, deviceName);
   std::ifstream devFile(devFilePath);
   if (devFile.fail())
   {
      // So we assume its a midi interface
      auto pDescr = std::make_shared<Description>();
      pDescr->hubSection.emplace(description::hub::Section{true, true});
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

template <typename T>
auto searchByDeviceId(const T &container, const MusicDeviceId &deviceId)
{
   auto it = container.find(deviceId.toStr());
   if (it == container.end())
   {
      for (auto iter = container.begin(); iter != container.end(); ++iter)
      {
         const MusicDeviceId keyAsId(iter->first, MusicDeviceId::ANY_PORT);
         if (keyAsId == deviceId)
         {
            it = iter;
         }
      }
   }
   return it;
}

void description::Loader::forEachDeviceInChain(
    const MusicDeviceId &rootDeviceId,
    std::function<void(const MusicDeviceId &nextDeviceId)> cb)
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }
   std::string port(rootDeviceId.toStr());
   for (const auto &deviceName : it->second)
   {
      cb(MusicDeviceId(deviceName, port));
      port = fmt::format("{}@{}", deviceName, port);
   }
}

void description::Loader::forFirstDeviceInChain(
    const MusicDeviceId &rootDeviceId,
    std::function<void(const MusicDeviceId &nextDeviceId)> cb)
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }
   std::string port(rootDeviceId.toStr());
   if (it->second.size())
   {
      cb(MusicDeviceId(it->second[0], port));
   }
}

struct ManufacturerEntry
{
   std::string manufacturer;
   std::vector<std::string> devices;
};
namespace meta
{
template<>
inline auto registerMembers<ManufacturerEntry>()
{
   return members(
      member("manufacturer", &ManufacturerEntry::manufacturer),
      member("devices", &ManufacturerEntry::devices)
   );
}
} // namespace meta

std::string description::Loader::getAllDevicesAsJson() const
{
   std::vector<ManufacturerEntry> ret;
   const std::string devicesPath =
       fmt::format("{}/MidiConfigs/Devices", m_configDir);

   for (const auto &manufacturerDir :
        std::filesystem::directory_iterator(devicesPath))
   {
      const std::string manufacturer =
          manufacturerDir.path().filename().string();
      const std::string devicePath =
          fmt::format("{}/MidiConfigs/Devices/{}", m_configDir, manufacturer);

      ManufacturerEntry manufacturerEntry;
      manufacturerEntry.manufacturer = manufacturer;
      for (const auto &deviceDir :
           std::filesystem::directory_iterator(devicePath))
      {
         const std::string device = deviceDir.path().filename().string();
         const std::string configFile =
             fmt::format("{}/Config.json", deviceDir.path().string());
         if (!std::filesystem::exists(configFile))
         {
            LOG_F(WARNING, "There is no Config.json present in config dir {}",
                  deviceDir.path().string());
            continue;
         }
         manufacturerEntry.devices.push_back(device);
      }
      if (!manufacturerEntry.devices.empty())
      {
         ret.push_back(manufacturerEntry);
      }
   }
   return meta::serialize(ret).dump();
}