#ifndef MUSIC_DEVICES_RPC_H
#define MUSIC_DEVICES_RPC_H

#include "MidiEmLoaderMusicDevicesRpcIf.h"

// clang-format off
namespace base::musicDevice::factory { class MusicDevices; }
// clang-format on

namespace uiadapter::capnzero
{
class MusicDevicesRpc : public ::capnzero::MidiEmLoader::MusicDevicesRpcIf
{
public:
   explicit MusicDevicesRpc(
       base::musicDevice::factory::MusicDevices& rMusicDevices);

   ::capnzero::Text requestMusicDeviceDescription(
       const ::capnzero::TextView& musicDeviceName) override;

private:
   base::musicDevice::factory::MusicDevices& m_rMusicDevices;
};

}   // namespace uiadapter::capnzero

#endif   // MUSIC_DEVICES_RPC_H
