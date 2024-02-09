#include "InstrumentComponent.h"
using namespace base;
using namespace base::instruments::loader;

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
         // spdlog::info("refreshing parameters");
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
    float increment, musicDevice::sound::IncrementMode incrementMode) const
{
   if (m_pSoundDevice)
   {
      const float actualValue =
          m_pParameterCache->getParameter(parameterIdx, parameterAttr);
      const auto valueRange = musicDevice::sound::getParamRangeEnd(
          m_sdVoiceIdx, parameterIdx, parameterAttr, *m_pSoundDevice);
      const float newParamValue = musicDevice::sound::limitParameterValue(
          actualValue + increment, incrementMode, valueRange);
      setParameterValue(parameterIdx, parameterAttr, newParamValue);
   }
}

void Component::incrementParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, musicDevice::sound::IncrementMode incrementMode) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_sdVoiceIdx, parameterIdx, parameterAttr, increment, incrementMode);
      m_pSoundDevice->lastplayerId = nullptr;
      m_pParameterCache->dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
   }
}

std::optional<float> Component::getParameterValue(
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
   return std::nullopt;
}

std::optional<float> Component::getSDParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->getParameterValue(m_sdVoiceIdx, parameterIdx,
                                               parameterAttr);
   }
   return std::nullopt;
}

void Component::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value) const
{
   if (m_pSoundDevice)
   {
      const auto valueRange = musicDevice::sound::getParamRangeEnd(
          m_sdVoiceIdx, parameterIdx, parameterAttr, *m_pSoundDevice);
      const float limitedValue =
          musicDevice::sound::limitParameterValue(value, musicDevice::sound::IncrementMode::Limit, valueRange);
      m_pParameterCache->setParameter(parameterIdx, parameterAttr,
                                      limitedValue);
      const float limitedModifiedValue =
          musicDevice::sound::limitParameterValue(
              m_pParameterCache->getModifiedParameterValue(parameterIdx,
                                                           parameterAttr),
              musicDevice::sound::IncrementMode::Limit, valueRange);
      m_pSoundDevice->setParameterValue(m_sdVoiceIdx, parameterIdx,
                                        parameterAttr, limitedModifiedValue);
   }
}

void Component::setParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value) const
{
   if (m_pSoundDevice)
   {
      const auto valueRange = musicDevice::sound::getParamRangeEnd(
          m_sdVoiceIdx, parameterIdx, parameterAttr, *m_pSoundDevice);
      const float limitedValue =
          musicDevice::sound::limitParameterValue(value, musicDevice::sound::IncrementMode::Limit, valueRange);

      m_pSoundDevice->setParameterValue(m_sdVoiceIdx, parameterIdx,
                                        parameterAttr, limitedValue);
      m_pSoundDevice->lastplayerId = nullptr;
      m_pParameterCache->dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
   }
}

float Component::fromNormalizedValue(
    int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->fromNormalizedValue(
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
      return &m_pSoundDevice->parameterDescription(m_sdVoiceIdx, parameterIdx);
   }
   return nullptr;
}

void Component::clearModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr) const
{
   m_pParameterCache->clearModifier(parameterIdx, parameterAttr);
}

void Component::applyModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr,
                              float destination, float intensity) const
{
   m_pParameterCache->applyModifier(parameterIdx, parameterAttr, destination,
                                    intensity);
}

void Component::refreshParameters() const
{
   if (!m_pParameterCache || !m_pSoundDevice)
   {
      return;
   }
   for (int paramIdx = 0; paramIdx < m_pParameterCache->size(); ++paramIdx)
   {
      if (m_pParameterCache->shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::Commanded))
      {
         m_pSoundDevice->setCommandedValue(
             m_sdVoiceIdx, paramIdx, m_pParameterCache->at(paramIdx).commanded, false);
      }
      if (m_pParameterCache->shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoAmplitude))
      {
         m_pSoundDevice->setLFOAmplitude(
             m_sdVoiceIdx, paramIdx,
             m_pParameterCache->at(paramIdx).lfo.amplitude, false);
      }
      if (m_pParameterCache->shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoFrequency))
      {
         m_pSoundDevice->setLFOFrequency(
             m_sdVoiceIdx, paramIdx,
             m_pParameterCache->at(paramIdx).lfo.frequency, false);
      }
      if (m_pParameterCache->shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoMultiplierExp))
      {
         m_pSoundDevice->setLFOMultiplierExp(
             m_sdVoiceIdx, paramIdx,
             m_pParameterCache->at(paramIdx).lfo.multiplierExp, false);
      }
      if (m_pParameterCache->shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoWaveform))
      {
         m_pSoundDevice->setLFOWaveform(
             m_sdVoiceIdx, paramIdx,
             m_pParameterCache->at(paramIdx).lfo.waveform, false);
      }
      m_pSoundDevice->calcActualVal(m_sdVoiceIdx, paramIdx);
   }
   m_pParameterCache->clearOverwriteList();
}
