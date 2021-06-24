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
    m_configDir(configDir.empty() ? "." : configDir),
    m_mapFileName(fmt::format("{}/MidiConfigs/usbMidiName2device.json", m_configDir)),
    m_deviceChainsFileName(fmt::format("{}/MidiConfigs/midiDeviceChains.json", m_configDir))
{
   std::ifstream mapFile(m_mapFileName);
   std::ifstream deviceChainsFile(m_deviceChainsFileName);
   if (mapFile.fail())
   {
      throw std::runtime_error(
          fmt::format("could not find file '{}'", m_mapFileName));
   }
   if (deviceChainsFile.fail())
   {
      LOG_F(INFO,
            "There is no custom midi interface connection config file '{}'",
            m_deviceChainsFileName);
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
               m_deviceChainsFileName, e.what());
      }
      catch (...)
      {
         LOG_F(ERROR, "ERROR at parsing ill formed '{}'", m_deviceChainsFileName);
      }
   }

   try
   {
      mapFile >> m_jUsbMidiName2deviceMap;
   }
   catch (...)
   {
      LOG_F(ERROR, "ERROR at parsing ill formed '{}'", m_mapFileName);
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
typename T::const_iterator searchByDeviceId(const T &container, const MusicDeviceId &deviceId)
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

template <typename T>
typename T::iterator searchByDeviceId(T &container, const MusicDeviceId &deviceId)
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
    std::function<void(const MusicDeviceId &nextDeviceId, uint8_t midiVoiceOffset)> cb)
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }
   std::string port(rootDeviceId.toStr());
   for (const auto &deviceDescr : it->second)
   {
      cb(MusicDeviceId(deviceDescr.musicDeviceName, port), deviceDescr.midiVoiceOffset);
      port = fmt::format("{}@{}", deviceDescr.musicDeviceName, port);
   }
}

void description::Loader::forFirstDeviceInChain(
    const MusicDeviceId &rootDeviceId,
    std::function<void(const MusicDeviceId &firstDeviceId, uint8_t midiVoiceOffset)> cb)
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }
   std::string port(rootDeviceId.toStr());
   if (it->second.size())
   {
      cb(MusicDeviceId(it->second[0].musicDeviceName, port), it->second[0].midiVoiceOffset);
   }
}

void description::Loader::forLastDeviceInChain(
    const MusicDeviceId &rootDeviceId,
    std::function<void(const MusicDeviceId &lastDeviceId, uint8_t midiVoiceOffset)> cb)
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }

   std::string port(rootDeviceId.toStr());
   for (int i = 0; i < it->second.size(); ++i)
   {
      if(i == (it->second.size() - 1))
      {
         cb(MusicDeviceId(it->second[i].musicDeviceName, port), it->second[i].midiVoiceOffset);
      }
      port = fmt::format("{}@{}", it->second[i].musicDeviceName, port);
   }
}

struct ManufacturerEntry
{
   std::string manufacturer;
   std::vector<std::string> devices;
};
namespace meta
{
template <> inline auto registerMembers<ManufacturerEntry>()
{
   return members(member("manufacturer", &ManufacturerEntry::manufacturer),
                  member("devices", &ManufacturerEntry::devices));
}
}   // namespace meta

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

void description::Loader::appendDeviceToChain(
    const MusicDeviceId &rootDeviceId, const MusicDeviceName &deviceName, uint8_t midiVoiceOffset) noexcept
{ 
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      m_deviceChains.deviceChains.insert(std::make_pair( rootDeviceId.toStr(), std::vector<DeviceChainsDeviceDescription>() ));
      it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   }
   it->second.push_back({deviceName, midiVoiceOffset});
   try{
      saveDeviceChainsToFile();
   } catch(std::exception& e){
      LOG_F(ERROR, "Exception occured at saving device chains to file: {}", e.what());
   }
}

void description::Loader::removeDeviceFromEndOf(
    const MusicDeviceId &rootDeviceId) noexcept
{
   auto it = searchByDeviceId(m_deviceChains.deviceChains, rootDeviceId);
   if (it == m_deviceChains.deviceChains.end())
   {
      return;
   }
   if(it->second.empty())
   {
      return;
   }
   it->second.pop_back();
   try{
      saveDeviceChainsToFile();
   } catch(std::exception& e){
      LOG_F(ERROR, "Exception occured at saving device chains to file: {}", e.what());
   }
}

void description::Loader::saveDeviceChainsToFile()
{
   std::ofstream deviceChainsFile(m_deviceChainsFileName);
   if (deviceChainsFile.fail())
   {
      LOG_F(INFO,
            "deviceChainsFile.fail() {}", m_deviceChainsFileName);
   }
   deviceChainsFile << meta::serialize(m_deviceChains).dump(3);
}