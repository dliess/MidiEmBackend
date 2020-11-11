#ifndef MUSIC_DEVICE_HOLDER_H
#define MUSIC_DEVICE_HOLDER_H

#include "MusicDeviceContainer.h"
#include "MidiHolder.h"

namespace base::musicDevice
{

struct Holder
{  
   MusicDeviceContainer musicDevices;
   MidiHolder midiHolder;
};

} // namespace base::musicDevice

#endif