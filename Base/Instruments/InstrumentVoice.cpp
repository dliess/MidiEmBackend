#include "InstrumentVoice.h"

using namespace base;
using namespace base::instruments;

void Voice::noteOn(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      if (m_pSoundDevice->lastplayerId !=
          static_cast<void*>(m_pParameterCache.get()))
      {
         // TODO: rectivate later
         // refreshParameters();
         m_pSoundDevice->lastplayerId =
             static_cast<void*>(m_pParameterCache.get());
      }
      m_pSoundDevice->noteOn(m_voiceIndex, note + m_noteOffset, velocity);
   }
}

void Voice::noteOff(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->noteOff(m_voiceIndex, note + m_noteOffset, velocity);
   }
}

void Voice::pitchBend(float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->pitchBend(m_voiceIndex, value);
   }
}

void Voice::incrementParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_voiceIndex, parameterIdx, parameterAttr, increment, roundRobin);
      musicDevice::sound::setParameterData(
          m_pParameterCache->at(parameterIdx), parameterAttr,
          m_pSoundDevice->getParameterValue(m_voiceIndex, parameterIdx,
                                            parameterAttr));
   }
}

void Voice::incrementParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_voiceIndex, parameterIdx, parameterAttr, increment, roundRobin);
   }
}

float Voice::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (m_pSoundDevice)
   {
      switch (parameterAttr)
      {
         case musicDevice::sound::ParameterAttr::Commanded:
            return m_pParameterCache->at(parameterIdx).commanded;
         case musicDevice::sound::ParameterAttr::LfoAmplitude:
            return m_pParameterCache->at(parameterIdx).lfo.amplitude;
         case musicDevice::sound::ParameterAttr::LfoFrequency:
            return m_pParameterCache->at(parameterIdx).lfo.frequency;
         case musicDevice::sound::ParameterAttr::LfoMultiplierExp:
            return float(m_pParameterCache->at(parameterIdx).lfo.multiplierExp);
         case musicDevice::sound::ParameterAttr::LfoWaveform:
            return float(static_cast<int>(
                m_pParameterCache->at(parameterIdx).lfo.waveform));
      }
   }
   return 0.0;   // TODO: return optional or inspect id m_pSoundDevice can be of
                 // type util::non_null
}

void Voice::setParameterValue(int parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr,
                              float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->setParameterValue(m_voiceIndex, parameterIdx,
                                        parameterAttr, value);
      musicDevice::sound::setParameterData(
          m_pParameterCache->at(parameterIdx), parameterAttr,
          m_pSoundDevice->getParameterValue(m_voiceIndex, parameterIdx,
                                            parameterAttr));
   }
}

float Voice::normalizePercentageValue(
    int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->normalizePercentageValue(
          m_voiceIndex, parameterId, parameterAttr, percentageValue);
   }
   return 0.0;   // TODO: return optional or inspect id m_pSoundDevice can be of
                 // type util::non_null
}

const musicDevice::description::sound::Parameter* Voice::parameterDescription(
    int parameterIdx) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->parameterDescription(m_voiceIndex, parameterIdx);
   }
   return nullptr;
}

void Voice::refreshParameters() const
{
   if (!m_pParameterCache || !m_pSoundDevice)
   {
      return;
   }
   for (int paramIdx = 0; paramIdx < m_pParameterCache->size(); ++paramIdx)
   {
      m_pSoundDevice->setCommandedValue(
          m_voiceIndex, paramIdx, m_pParameterCache->at(paramIdx).commanded);
      m_pSoundDevice->setLFOAmplitude(
          m_voiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.amplitude);
      m_pSoundDevice->setLFOFrequency(
          m_voiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.frequency);
      m_pSoundDevice->setLFOMultiplierExp(
          m_voiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.multiplierExp);
      m_pSoundDevice->setLFOWaveform(
          m_voiceIndex, paramIdx, m_pParameterCache->at(paramIdx).lfo.waveform);
   }
}
