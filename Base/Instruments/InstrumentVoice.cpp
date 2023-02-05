#include "InstrumentVoice.h"

using namespace base::instruments;

/*
Voice::Voice(musicDevice::sound::SoundHandler& sh,
             util::Identifiable::UUIDView uuid, int voiceIdx, int noteOffset) :
    pSoundDevice(&sh),
    pParameterCache(),
    soundDeviceId(uuid),
    voiceIndex(voiceIdx),
    noteOffset(noteOffset)
{
}

*/

void Voice::noteOn(int note, float velocity) const
{
   if (pSoundDevice)
   {
      if (pSoundDevice->lastplayerId !=
          static_cast<void*>(pParameterCache.get()))
      {
         refreshParameters();
         pSoundDevice->lastplayerId = static_cast<void*>(pParameterCache.get());
      }
      pSoundDevice->noteOn(voiceIndex, note + noteOffset, velocity);
   }
}

void Voice::noteOff(int note, float velocity) const
{
   if (pSoundDevice)
   {
      pSoundDevice->noteOff(voiceIndex, note + noteOffset, velocity);
   }
}

void Voice::refreshParameters() const
{
   if (!pParameterCache || !pSoundDevice)
   {
      return;
   }
   for (int paramIdx = 0; paramIdx < pParameterCache->size(); ++paramIdx)
   {
      pSoundDevice->setParameterValue(voiceIndex, paramIdx,
                                      pParameterCache->at(paramIdx).commanded);
      pSoundDevice->setLFOAmplitude(
          voiceIndex, paramIdx,
          pParameterCache->at(paramIdx).lfoData.amplitude);
      pSoundDevice->setLFOFrequency(
          voiceIndex, paramIdx,
          pParameterCache->at(paramIdx).lfoData.frequency);
      pSoundDevice->setLFOMultiplierExp(
          voiceIndex, paramIdx,
          pParameterCache->at(paramIdx).lfoData.multiplierExp);
      pSoundDevice->setLFOWaveform(
          voiceIndex, paramIdx, pParameterCache->at(paramIdx).lfoData.waveform);
   }
}
