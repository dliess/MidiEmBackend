#include "MusicDevicesRpc.h"

#include <nlohmann/json.hpp>

#include "MusicDeviceFactoryDataHolder.h"

using namespace uiadapter::capnzero;

MusicDevicesRpc::MusicDevicesRpc(
    base::musicDevice::factory::MusicDevices& rMusicDevices) :
    m_rMusicDevices(rMusicDevices)
{
}

::capnzero::Text MusicDevicesRpc::requestMusicDeviceDescription(
    const ::capnzero::TextView& musicDeviceName)
{
   auto pDescr =
       m_rMusicDevices.getDescription(std::string(musicDeviceName));
   if (pDescr)
   {
      return nlohmann::json(*pDescr).dump();
   }
   return "";
}
