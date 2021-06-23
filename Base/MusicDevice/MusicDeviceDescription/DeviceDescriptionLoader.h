#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include <string>

#include "MusicDeviceChains.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"
#include "json.hpp"

namespace base::musicDevice
{
namespace description
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
       const std::string& deviceName) const noexcept;
   std::shared_ptr<Description> load(const std::string& deviceName) const;

   void forEachDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& nextDeviceId)> cb);

   void forFirstDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& firstDeviceId)> cb);

   void forLastDeviceInChain(
       const MusicDeviceId& rootDeviceId,
       std::function<void(const MusicDeviceId& lastDeviceId)> cb);

   std::string getAllDevicesAsJson() const;
   void appendDeviceToChain(const MusicDeviceId& rootDeviceId,
                            const MusicDeviceName& deviceName) noexcept;
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

}   // namespace description
}   // namespace base::musicDevice

#endif