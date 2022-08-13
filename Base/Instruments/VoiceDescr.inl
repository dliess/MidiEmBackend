#include "VoiceDescr.h"

namespace base::instruments
{

inline
void VoiceDescr::updateMusicDevicePtr(
   musicDevice::Holder& rMusicDeviceHolder) noexcept
{
   auto iter = rMusicDeviceHolder.musicDevices.findByDeviceId(soundDeviceId);
   if (rMusicDeviceHolder.musicDevices.end() != iter)
   {
      pSoundDevice = iter->second;
   }
}

}