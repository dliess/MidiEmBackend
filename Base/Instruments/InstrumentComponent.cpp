#include "InstrumentComponent.h"

using namespace base;
using namespace base::instruments;

void Component::noteOn(int note, float velocity) const
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
      m_pSoundDevice->noteOn(m_sdVoiceIndex, note + m_noteOffset, velocity);
   }
}

void Component::noteOff(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->noteOff(m_sdVoiceIndex, note + m_noteOffset, velocity);
   }
}

void Component::pitchBend(float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->pitchBend(m_sdVoiceIndex, value);
   }
}

void Component::incrementParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_sdVoiceIndex, parameterIdx, parameterAttr, increment, roundRobin);
      if (m_pParameterCache)
      {
         m_pParameterCache->setParameter(
             parameterIdx, parameterAttr,
             m_pSoundDevice->getParameterValue(m_sdVoiceIndex, parameterIdx,
                                               parameterAttr));
      }
   }
}

void Component::incrementParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_sdVoiceIndex, parameterIdx, parameterAttr, increment, roundRobin);
   }
}

float Component::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (m_pParameterCache)
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

float Component::getParameterValueCached(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->getParameterValue(m_sdVoiceIndex, parameterIdx,
                                               parameterAttr);
   }
   return 0.0;   // TODO: better error handling
}

void Component::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->setParameterValue(m_sdVoiceIndex, parameterIdx,
                                        parameterAttr, value);
      if (m_pParameterCache)
      {
         m_pParameterCache->setParameter(
             parameterIdx, parameterAttr,
             m_pSoundDevice->getParameterValue(m_sdVoiceIndex, parameterIdx,
                                               parameterAttr));
      }
   }
}

float Component::normalizePercentageValue(
    int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->normalizePercentageValue(
          m_sdVoiceIndex, parameterId, parameterAttr, percentageValue);
   }
   return 0.0;   // TODO: return optional or inspect id m_pSoundDevice can be of
                 // type util::non_null
}

const musicDevice::description::sound::Parameter*
Component::parameterDescription(int parameterIdx) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->parameterDescription(m_sdVoiceIndex, parameterIdx);
   }
   return nullptr;
}

void Component::refreshParameters() const
{
   if (!m_pParameterCache || !m_pSoundDevice)
   {
      return;
   }
   for (int paramIdx = 0; paramIdx < m_pParameterCache->size(); ++paramIdx)
   {
      m_pSoundDevice->setCommandedValue(
          m_sdVoiceIndex, paramIdx, m_pParameterCache->at(paramIdx).commanded);
      m_pSoundDevice->setLFOAmplitude(
          m_sdVoiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.amplitude);
      m_pSoundDevice->setLFOFrequency(
          m_sdVoiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.frequency);
      m_pSoundDevice->setLFOMultiplierExp(
          m_sdVoiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.multiplierExp);
      m_pSoundDevice->setLFOWaveform(
          m_sdVoiceIndex, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.waveform);
   }
}
