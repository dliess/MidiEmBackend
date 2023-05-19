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
         //refreshParameters();
         m_pSoundDevice->lastplayerId = static_cast<void*>(m_pParameterCache.get());
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

void Voice::incrementParameterValue(int parameterIdx, float increment,
                                    bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(m_voiceIndex, parameterIdx, increment,
                                            roundRobin);
      m_pParameterCache->data.at(parameterIdx).commanded =
          m_pSoundDevice->getParameterValue(m_voiceIndex, parameterIdx);
   }
}

void Voice::incrementParameterValueDontCache(int parameterIdx, float increment,
                                    bool roundRobin) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(m_voiceIndex, parameterIdx, increment,
                                            roundRobin);
   }
}

float Voice::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterPart parameterPart) const
{
   if (m_pSoundDevice)
   {
      switch (parameterPart)
      {
         case musicDevice::sound::ParameterPart::Commanded:
            return m_pParameterCache->data.at(parameterIdx).commanded;
         case musicDevice::sound::ParameterPart::LfoAmplitude:
            return m_pParameterCache->data.at(parameterIdx).lfoData.amplitude;
         case musicDevice::sound::ParameterPart::LfoFrequency:
            return m_pParameterCache->data.at(parameterIdx).lfoData.frequency;
         case musicDevice::sound::ParameterPart::LfoMultiplierExp:
            return m_pParameterCache->data.at(parameterIdx).lfoData.multiplierExp;
         case musicDevice::sound::ParameterPart::LfoWaveform:
            return static_cast<int>(
                m_pParameterCache->data.at(parameterIdx).lfoData.waveform);
      }
   }
   return 0.0;   // TODO: return optional or inspect id m_pSoundDevice can be of
                 // type util::non_null
}

void Voice::setParameterValue(int parameterIdx, float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->setParameterValue(m_voiceIndex, parameterIdx, value);
      m_pParameterCache->data.at(parameterIdx).commanded = value;
   }
}

float Voice::normalizePercentageValue(
    int parameterId, musicDevice::sound::ParameterPart parameterPart,
    float percentageValue) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->normalizePercentageValue(
          m_voiceIndex, parameterId, parameterPart, percentageValue);
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
   for (int paramIdx = 0; paramIdx < m_pParameterCache->data.size(); ++paramIdx)
   {
      m_pSoundDevice->setParameterValue(m_voiceIndex, paramIdx,
                                      m_pParameterCache->data.at(paramIdx).commanded);
      m_pSoundDevice->setLFOAmplitude(
          m_voiceIndex, paramIdx,
          m_pParameterCache->data.at(paramIdx).lfoData.amplitude);
      m_pSoundDevice->setLFOFrequency(
          m_voiceIndex, paramIdx,
          m_pParameterCache->data.at(paramIdx).lfoData.frequency);
      m_pSoundDevice->setLFOMultiplierExp(
          m_voiceIndex, paramIdx,
          m_pParameterCache->data.at(paramIdx).lfoData.multiplierExp);
      m_pSoundDevice->setLFOWaveform(
          m_voiceIndex, paramIdx, m_pParameterCache->data.at(paramIdx).lfoData.waveform);
   }
}
