#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include <string>

#include "MusicDeviceChains.h"
#include "MusicDeviceDescription.h"
#include "json.hpp"


namespace base::musicDevice
{
struct MusicDeviceId;

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
   std::pair<ResultType, std::string> getMatchType(const std::string& deviceName) const noexcept;
   std::shared_ptr<Description> load(const std::string& deviceName) const;

   void forEachDeviceInChain(
      const MusicDeviceId& rootDeviceId,
      std::function<void(const MusicDeviceId& nextDeviceId)> cb);

   void forFirstDeviceInChain(
      const MusicDeviceId& rootDeviceId,
      std::function<void(const MusicDeviceId& nextDeviceId)> cb);

private:
   std::string m_configDir;
   using json = nlohmann::json;
   json m_jUsbMidiName2deviceMap;
   DeviceChains m_deviceChains; // TODO: put device-chain handling in separate class
};

} // namespace description
} // namespace base::musicDevice

#endif