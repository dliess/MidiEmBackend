#ifndef MUSIC_DEVICE_CHAINS_H
#define MUSIC_DEVICE_CHAINS_H

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include "MusicDeviceId.h"

/* Sample midiDeviceChains.json
{
   "deviceChain" : [
      "MIDI4x4:MIDI4x4 MIDI 3@1:4" : {
         manufacturer: "Elektron",
         device: "Octatrack_Mk1",
         next: [{
            manufacturer: "Mitxela",
            device: "Flash"
         }]
      }
   ]
}
*/

namespace base::musicDevice
{

struct DeviceChainsDeviceDescription
{
   MusicDeviceName musicDeviceName;
   uint8_t midiVoiceOffset{0};
};
struct DeviceChains
{
   std::unordered_map<std::string, std::vector<DeviceChainsDeviceDescription>> deviceChains;
};

} // namespace base::musicDevice

#include "MusicDeviceChainsMeta.h"

#endif