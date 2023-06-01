#ifndef MUSIC_DEVICES_RPC_H
#define MUSIC_DEVICES_RPC_H

#include "MidiEmLoaderMusicDevicesRpcIf.h"

// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
// clang-format on

namespace uiadapter::capnzero
{
class MusicDevicesRpc : public ::capnzero::MidiEmLoader::MusicDevicesRpcIf
{
public:
   explicit MusicDevicesRpc(
       base::musicDevice::factory::DataHolder& rFactoryDataHolder);

   ::capnzero::Text requestMusicDeviceDescription(
       const ::capnzero::TextView& musicDeviceName) override;

private:
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
};

}   // namespace uiadapter::capnzero

#endif   // MUSIC_DEVICES_RPC_H