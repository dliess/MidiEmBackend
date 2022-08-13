#ifndef INSTRUMENTS_VOICE_DESCR_H
#define INSTRUMENTS_VOICE_DESCR_H

#include "Identifiable.h"
#include "MusicDevice.h"
#include "SoundHandler.h"

namespace base::instruments
{

struct VoiceDescr : public util::Identifiable
{
   musicDevice::sound::SoundHandler* pSoundDevice{nullptr};
   musicDevice::MusicDeviceId soundDeviceId;
   int voiceIndex{0};
   int noteOffset{0};
};

}   // namespace base::instruments

#include "VoiceDescr.inl"

#endif