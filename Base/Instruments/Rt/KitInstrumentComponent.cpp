#include "KitInstrumentComponent.h"
using namespace base;
using namespace base::instruments::rt;

void KitComponent::noteOn(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      if (m_pSoundDevice->lastplayerId !=
          static_cast<const void*>(this))
      {
         refreshParameters();
         m_pSoundDevice->lastplayerId =
             static_cast<const void*>(this);
      }
      m_pSoundDevice->noteOn(m_sdVoiceIdx, note + m_noteOffset, velocity);
   }
}

void KitComponent::noteOff(int note, float velocity) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->noteOff(m_sdVoiceIdx, note + m_noteOffset, velocity);
   }
}

void KitComponent::pitchBend(float value) const
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->pitchBend(m_sdVoiceIdx, value);
   }
}

Void KitComponent::incrementParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, musicDevice::sound::IncrementMode incrementMode)
{
   if (m_pSoundDevice)
   {
      return m_parameterCache.getParameter(parameterIdx, parameterAttr).map(
          [&,this](float actualValue) {
             const auto valueRange = musicDevice::sound::getParamRangeEnd(
                 m_sdVoiceIdx, parameterIdx, parameterAttr, *m_pSoundDevice);
             const float newParamValue = musicDevice::sound::limitParameterValue(
                 actualValue + increment, incrementMode, valueRange);
             setParameterValue(parameterIdx, parameterAttr, newParamValue);
          }
      );
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Void KitComponent::incrementParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float increment, musicDevice::sound::IncrementMode incrementMode)
{
   if (m_pSoundDevice)
   {
      m_pSoundDevice->incrementParameterValue(
          m_sdVoiceIdx, parameterIdx, parameterAttr, increment, incrementMode);
      m_pSoundDevice->lastplayerId = nullptr;
      m_parameterCache.dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
      return Void();
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Ret<float> KitComponent::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
      switch (parameterAttr)
      {
         case musicDevice::sound::ParameterAttr::Commanded:
            return m_parameterCache.at(parameterIdx).map([](auto p){ return p->commanded; });
         case musicDevice::sound::ParameterAttr::LfoAmplitude:
            return m_parameterCache.at(parameterIdx).map([](auto p){ return p->lfo.amplitude; });
         case musicDevice::sound::ParameterAttr::LfoFrequency:
            return m_parameterCache.at(parameterIdx).map([](auto p){ return p->lfo.frequency; });
         case musicDevice::sound::ParameterAttr::LfoMultiplierExp:
            return m_parameterCache.at(parameterIdx).map([](auto p) { return float(p->lfo.multiplierExp); });
         case musicDevice::sound::ParameterAttr::LfoWaveform:
            return m_parameterCache.at(parameterIdx).map([](auto p){ return float(static_cast<int>(p->lfo.waveform)); });
      }
   return tl::unexpected(Error::indexOutOfRange);
}

Ret<float> KitComponent::getSDParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (m_pSoundDevice)
   {
      const auto val = m_pSoundDevice->getParameterValue(m_sdVoiceIdx, parameterIdx,
                                               parameterAttr);
      if(val) return val.value();
      return tl::unexpected(Error::indexOutOfRange);
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Void KitComponent::setParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   if (m_pSoundDevice)
   {
      const auto valueRange = musicDevice::sound::getParamRangeEnd(
          m_sdVoiceIdx, parameterIdx, parameterAttr, *m_pSoundDevice);
      const float limitedValue =
          musicDevice::sound::limitParameterValue(value, musicDevice::sound::IncrementMode::Limit, valueRange);
      m_parameterCache.setParameter(parameterIdx, parameterAttr,
                                      limitedValue);
      
      return m_parameterCache.getModifiedParameterValue(parameterIdx,
                                                        parameterAttr).map(
          [&,this](float modifiedValue) {
          const float limitedModifiedValue =
              musicDevice::sound::limitParameterValue(
                  modifiedValue,  musicDevice::sound::IncrementMode::Limit, valueRange);
             m_pSoundDevice->setParameterValue(m_sdVoiceIdx, parameterIdx,
                                               parameterAttr, limitedModifiedValue);
          }
      );
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Void KitComponent::setParameterValueDontCache(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
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
      m_parameterCache.dontOverwriteOnNextNoteOn(parameterIdx, parameterAttr);
      return Void{};
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Ret<float> KitComponent::fromNormalizedValue(
    int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (m_pSoundDevice)
   {
      return m_pSoundDevice->fromNormalizedValue(
          m_sdVoiceIdx, parameterId, parameterAttr, percentageValue);
   }
   return tl::unexpected(Error::noSoundDeviceCached);
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

Void KitComponent::clearModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr)
{
   return m_parameterCache.clearModifier(parameterIdx, parameterAttr);
}

Void KitComponent::applyModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr,
                              float destination, float intensity)
{
   return m_parameterCache.applyModifier(parameterIdx, parameterAttr, destination,
                                    intensity);
}

Void KitComponent::refreshParameters() const
{
   if (!m_pSoundDevice)
   {
      return tl::unexpected(Error::noSoundDeviceCached);
   }
   for (int paramIdx = 0; paramIdx < m_parameterCache.size(); ++paramIdx)
   {
      if (m_parameterCache.shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::Commanded))
      {
         return m_parameterCache.getParameter(paramIdx, musicDevice::sound::ParameterAttr::Commanded).map(
             [&,this](float commandedValue) {
                m_pSoundDevice->setCommandedValue(
                    m_sdVoiceIdx, paramIdx, commandedValue, false);
             }
         );
      }
      if (m_parameterCache.shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoAmplitude))
      {
         return m_parameterCache.getParameter(paramIdx, musicDevice::sound::ParameterAttr::LfoAmplitude).map(
             [&,this](float lfoAmplitude) {
                m_pSoundDevice->setLFOAmplitude(
                    m_sdVoiceIdx, paramIdx, lfoAmplitude, false);
             }
         );
      }
      if (m_parameterCache.shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoFrequency))
      {
         return m_parameterCache.getParameter(paramIdx, musicDevice::sound::ParameterAttr::LfoFrequency).map(
             [&,this](float lfoFrequency) {
                m_pSoundDevice->setLFOFrequency(
                    m_sdVoiceIdx, paramIdx, lfoFrequency, false);
             }
         );
      }
      if (m_parameterCache.shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoMultiplierExp))
      {
         return m_parameterCache.getParameter(paramIdx, musicDevice::sound::ParameterAttr::LfoMultiplierExp).map(
             [&,this](float lfoMultiplierExp) {
                m_pSoundDevice->setLFOMultiplierExp(
                    m_sdVoiceIdx, paramIdx, lfoMultiplierExp, false);
             }
         );
      }
      if (m_parameterCache.shouldBeOverwritten(
              paramIdx, musicDevice::sound::ParameterAttr::LfoWaveform))
      {
         return m_parameterCache.getParameter(paramIdx, musicDevice::sound::ParameterAttr::LfoWaveform).map(
             [&,this](float lfoWaveform) {
                m_pSoundDevice->setLFOWaveform(
                    m_sdVoiceIdx, paramIdx, static_cast<base::musicDevice::sound::lfo::Waveform>(int(lfoWaveform)), false);
             }
         );
      }
      m_pSoundDevice->calcActualVal(m_sdVoiceIdx, paramIdx);
   }
   m_parameterCache.clearOverwriteList();
   return Void{};
}
