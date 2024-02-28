#ifndef INSTRUMENTS_SD_VOICE_REF_H
#define INSTRUMENTS_SD_VOICE_REF_H

#include "SoundHandler.h"
#include "MusicDeviceId.h"

namespace base::instruments::rt
{

struct SdVoiceRef
{
   explicit SdVoiceRef(musicDevice::MusicDeviceId soundDeviceId, 
                       int sdVoiceIdx, 
                       musicDevice::sound::SoundHandler* soundHandler) noexcept :
      soundDeviceId(soundDeviceId),
      sdVoiceIdx(sdVoiceIdx),
      soundHandler(soundHandler) {}
   musicDevice::MusicDeviceId soundDeviceId;
   int sdVoiceIdx{0};
   musicDevice::sound::SoundHandler* soundHandler{nullptr};
};

}   // namespace base::instruments::rt

#endif // ! INSTRUMENTS_SD_VOICE_REF_H
