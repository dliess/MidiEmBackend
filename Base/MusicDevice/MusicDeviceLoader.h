#ifndef DEVICE_LOADER_H
#define DEVICE_LOADER_H

#include <nlohmann/json.hpp>
#include <string>

#include "MusicDeviceChains.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"
#include "RtMidiAdaptTypes.h"

namespace base::musicDevice
{
class Loader
{
public:
   enum class ResultType
   {
      NotFound,
      MusicDevice,
      MarkedUnused
   };

   Loader(const std::string& configDir);
   std::pair<ResultType, std::string> getMatchType(
       const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) const noexcept;
   std::shared_ptr<description::Description> load(
       const std::string& deviceName) const;

   void forEachDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& nextDeviceId,
                          uint8_t midiVoiceOffset)>
           cb);

   void forFirstDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& firstDeviceId,
                          uint8_t midiVoiceOffset)>
           cb);

   void forLastDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& lastDeviceId,
                          uint8_t midiVoiceOffset)>
           cb);

   std::string getAllDevicesAsJson() const;
   void appendDeviceToChain(const MusicDeviceId& rootDeviceId,
                            const MusicDeviceName& deviceName,
                            uint8_t midiVoiceOffset) noexcept;
   void removeDeviceFromEndOf(const MusicDeviceId& rootDeviceId) noexcept;

private:
   const std::string m_configDir;
   const std::string m_mapFileName;
   const std::string m_deviceChainsFileName;
   using json = nlohmann::json;
   json m_jUsbMidiName2deviceMap;
   DeviceChains
       m_deviceChains;   // TODO: put device-chain handling in separate class
   void saveDeviceChainsToFile();
};

}   // namespace base::musicDevice

#endif