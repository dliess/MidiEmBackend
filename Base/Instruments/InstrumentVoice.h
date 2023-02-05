#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include <memory>
#include <vector>

#include "MusicDevice.h"
#include "ParameterData.h"
#include "SoundHandler.h"

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
   void incrementParameterValue(int parameterIdx, float increment,
                                bool roundRobin) const;
   [[nodiscard]] float getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterPart parameterPart) const;
   void setParameterValue(int parameterIdx, float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int parameterId, musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const;

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
