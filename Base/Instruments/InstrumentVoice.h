#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include "Identifiable.h"
#include "MusicDevice.h"
#include "SoundHandler.h"

namespace base::instruments
{

struct Voice : public util::Identifiable
{
   musicDevice::sound::SoundHandler* pSoundDevice{nullptr};
   musicDevice::MusicDeviceId soundDeviceId;
   int voiceIndex{0};
   int noteOffset{0};
};

}   // namespace base::instruments

#endif