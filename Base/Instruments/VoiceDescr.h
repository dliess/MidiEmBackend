#ifndef INSTRUMENTS_VOICE_DESCR_H
#define INSTRUMENTS_VOICE_DESCR_H

#include "Identifiable.h"
#include "MusicDevice.h"
#include "MusicDeviceHolder.h"

namespace base::instruments
{

struct VoiceDescr : public util::Identifiable
{
   musicDevice::MusicDeviceId soundDeviceId;
   std::shared_ptr<musicDevice::MusicDevice> pSoundDevice;
   int voiceIndex;
   int noteOffset;
   inline void updateMusicDevicePtr(musicDevice::Holder& rMusicDeviceHolder) noexcept;
};

}   // namespace base::instruments

#include "VoiceDescr.inl"

#endif