#include "KitInstrumentComponent.h"
using namespace base;
using namespace base::instruments::loader;

std::optional<float> KitComponent::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   switch (parameterAttr)
   {
      case musicDevice::sound::ParameterAttr::Commanded:
         return m_parameterData.at(parameterIdx).commanded;
      case musicDevice::sound::ParameterAttr::LfoAmplitude:
         return m_parameterData.at(parameterIdx).lfo.amplitude;
      case musicDevice::sound::ParameterAttr::LfoFrequency:
         return m_parameterData.at(parameterIdx).lfo.frequency;
      case musicDevice::sound::ParameterAttr::LfoMultiplierExp:
         return float(m_parameterData.at(parameterIdx).lfo.multiplierExp);
      case musicDevice::sound::ParameterAttr::LfoWaveform:
         return float(static_cast<int>(
             m_parameterData.at(parameterIdx).lfo.waveform));
   }
   return std::nullopt;
}

void KitComponent::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value) 
{
    musicDevice::sound::setParameterData(m_parameterData.at(parameterIdx), parameterAttr,
                                        value);
}


const musicDevice::description::sound::Parameter*
KitComponent::parameterDescription(int parameterIdx) const
{
   if (m_pSoundDevice)
   {
      return &m_pSoundDevice->parameterDescription(m_sdVoiceIdx, parameterIdx);
   }
   return nullptr;
}

