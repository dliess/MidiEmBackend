#include "MusicDevicesRpc.h"

#include <nlohmann/json.hpp>

#include "MusicDeviceFactoryDataHolder.h"

using namespace uiadapter::capnzero;

MusicDevicesRpc::MusicDevicesRpc(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}

::capnzero::Text MusicDevicesRpc::requestMusicDeviceDescription(
    const ::capnzero::TextView& musicDeviceName)
{
   auto pDescr =
       m_rFactoryDataHolder.getDescription(std::string(musicDeviceName));
   if (pDescr)
   {
      return nlohmann::json(*pDescr).dump();
   }
   return "";
}