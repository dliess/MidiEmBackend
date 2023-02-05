#include "InstrumentVoice.h"

using namespace base;
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

void Voice::incrementParameterValue(int parameterIdx, float increment,
                                    bool roundRobin) const
{
   if (pSoundDevice)
   {
      pSoundDevice->incrementParameterValue(voiceIndex, parameterIdx, increment,
                                            roundRobin);
      pParameterCache->at(parameterIdx).commanded =
          pSoundDevice->getParameterValue(voiceIndex, parameterIdx);
   }
}

float Voice::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterPart parameterPart) const
{
   if (pSoundDevice && pParameterCache)
   {
      switch (parameterPart)
      {
         case musicDevice::sound::ParameterPart::Commanded:
            return pParameterCache->at(parameterIdx).commanded;
         case musicDevice::sound::ParameterPart::LfoAmplitude:
            return pParameterCache->at(parameterIdx).lfoData.amplitude;
         case musicDevice::sound::ParameterPart::LfoFrequency:
            return pParameterCache->at(parameterIdx).lfoData.frequency;
         case musicDevice::sound::ParameterPart::LfoMultiplierExp:
            return pParameterCache->at(parameterIdx).lfoData.multiplierExp;
         case musicDevice::sound::ParameterPart::LfoWaveform:
            return static_cast<int>(
                pParameterCache->at(parameterIdx).lfoData.waveform);
      }
   }
   return 0.0;   // TODO: return optional or inspect id pSoundDevice can be of
                 // type util::non_null
}

void Voice::setParameterValue(int parameterIdx, float value) const
{
   if (pSoundDevice)
   {
      pSoundDevice->setParameterValue(voiceIndex, parameterIdx, value);
      pParameterCache->at(parameterIdx).commanded = value;
   }
}

float Voice::normalizePercentageValue(
    int parameterId, musicDevice::sound::ParameterPart parameterPart,
    float percentageValue) const
{
   if (pSoundDevice)
   {
      return pSoundDevice->normalizePercentageValue(
          voiceIndex, parameterId, parameterPart, percentageValue);
   }
   return 0.0;   // TODO: return optional or inspect id pSoundDevice can be of
                 // type util::non_null
}

const musicDevice::description::sound::Parameter* Voice::parameterDescription(
    int parameterIdx) const
{
   if (pSoundDevice)
   {
      return pSoundDevice->parameterDescription(voiceIndex, parameterIdx);
   }
   return nullptr;
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
