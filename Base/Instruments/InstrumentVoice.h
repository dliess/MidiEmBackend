#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include "MusicDevice.h"
#include "SoundHandler.h"
#include "ParameterData.h"
#include <memory>
#include <vector>

namespace base::instruments
{

struct Voice
{
      /*
   Voice(musicDevice::sound::SoundHandler& sh, 
         util::Identifiable::UUIDView uuid,
         int voiceIdx, int noteOffset);
         */
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   musicDevice::sound::SoundHandler* pSoundDevice{nullptr};
   using ParameterCache = std::vector<base::musicDevice::sound::ParameterData>;
   std::shared_ptr<ParameterCache> pParameterCache;
   musicDevice::MusicDeviceId soundDeviceId;
   int voiceIndex{0};
   int noteOffset{0};
private:
   void refreshParameters() const;
};

}   // namespace base::instruments

#endif
