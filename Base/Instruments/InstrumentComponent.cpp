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
         refreshParameters();
         m_pSoundDevice->lastplayerId =
             static_cast<void*>(m_pParameterCache.get());
         //spdlog::info("refreshing parameters");
      }
      m_pSoundDevice->noteOn(m_sdVoiceIdx, note + m_noteOffset, velocity);
   }
}

void Component::noteOff(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->noteOff(m_sdVoiceIdx, note + m_noteOffset, velocity);
   }
}

void Component::pitchBend(float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->pitchBend(m_sdVoiceIdx, value);
   }
}

void Component::incrementParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, bool roundRobin) const
{ /*TODO: increment paramcache first and then set sounddevice*/
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_sdVoiceIdx, parameterIdx, parameterAttr, increment, roundRobin);
      if (m_pParameterCache)
      {
         m_pParameterCache->setParameter(
             parameterIdx, parameterAttr,
             m_pSoundDevice->getParameterValue(m_sdVoiceIdx, parameterIdx,
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
          m_sdVoiceIdx, parameterIdx, parameterAttr, increment, roundRobin);
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
      return m_pSoundDevice->getParameterValue(m_sdVoiceIdx, parameterIdx,
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
      m_pSoundDevice->setParameterValue(m_sdVoiceIdx, parameterIdx,
                                        parameterAttr, value);
      if (m_pParameterCache)
      {
         m_pParameterCache->setParameter(
             parameterIdx, parameterAttr,
             m_pSoundDevice->getParameterValue(m_sdVoiceIdx, parameterIdx,
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
          m_sdVoiceIdx, parameterId, parameterAttr, percentageValue);
   }
   return 0.0;   // TODO: return optional or inspect id m_pSoundDevice can be of
                 // type util::non_null
}

const musicDevice::description::sound::Parameter*
Component::parameterDescription(int parameterIdx) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->parameterDescription(m_sdVoiceIdx, parameterIdx);
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
          m_sdVoiceIdx, paramIdx, m_pParameterCache->at(paramIdx).commanded);
      m_pSoundDevice->setLFOAmplitude(
          m_sdVoiceIdx, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.amplitude);
      m_pSoundDevice->setLFOFrequency(
          m_sdVoiceIdx, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.frequency);
      m_pSoundDevice->setLFOMultiplierExp(
          m_sdVoiceIdx, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.multiplierExp);
      m_pSoundDevice->setLFOWaveform(
          m_sdVoiceIdx, paramIdx,
          m_pParameterCache->at(paramIdx).lfo.waveform);
   }
}
