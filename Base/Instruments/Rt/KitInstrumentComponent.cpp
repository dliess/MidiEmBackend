#include "KitInstrumentComponent.h"
#include "magic_enum.hpp"
using namespace base;
using namespace base::instruments::rt;

KitComponent::KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIdx, int noteOffset) noexcept :
    sdVoiceRef(soundDeviceId, sdVoiceIdx, pSoundDevice),
    data(pSoundDevice->description().engineBase(sdVoiceIdx))
{
   data.noteOffset = noteOffset;
}

bool KitComponent::setNoteOffset(int noteOffset)
{
   if(data.noteOffset != noteOffset)
   {
      data.noteOffset = noteOffset;
      return true;
   }
   return false;
}

bool KitComponent::setAmp(float amp, float prevAmp)
{
   if(data.amp != amp)
   {
      data.amp = amp;
      if(sdVoiceRef.soundHandler)
      {
         sdVoiceRef.soundHandler->setAmp(sdVoiceRef.sdVoiceIdx, prevAmp * data.amp);
      }
      return true;
   }
   return false;
}

void KitComponent::updateParameterUI()
{
   data.parameterCache.updateParameterUI();
}

void KitComponent::noteOn(int note, float velocity, util::function_ref<void()> refreshParameters) const
{
   if (sdVoiceRef.soundHandler)
   {
      if (sdVoiceRef.soundHandler->lastplayerId !=
          static_cast<const void*>(this))
      {
         refreshParameters();
         sdVoiceRef.soundHandler->lastplayerId =
             static_cast<const void*>(this);
      }
      sdVoiceRef.soundHandler->noteOn(sdVoiceRef.sdVoiceIdx, note + data.noteOffset, velocity);
   }
}

void KitComponent::noteOff(int note, float velocity) const
{
   if (sdVoiceRef.soundHandler)
   {
      sdVoiceRef.soundHandler->noteOff(sdVoiceRef.sdVoiceIdx, note + data.noteOffset, velocity);
   }
}

void KitComponent::pitchBend(float value) const
{
   if (sdVoiceRef.soundHandler)
   {
      sdVoiceRef.soundHandler->pitchBend(sdVoiceRef.sdVoiceIdx, value);
   }
}

Ret<float> KitComponent::getParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
      switch (parameterAttr)
      {
         case musicDevice::sound::ParameterAttr::Commanded:
            return data.parameterCache.at(parameterIdx).map([](auto p){ return p->commanded; });
         case musicDevice::sound::ParameterAttr::LfoAmplitude:
            return data.parameterCache.at(parameterIdx).map([](auto p){ return p->lfo.amplitude; });
         case musicDevice::sound::ParameterAttr::LfoFrequency:
            return data.parameterCache.at(parameterIdx).map([](auto p){ return p->lfo.frequency; });
         case musicDevice::sound::ParameterAttr::LfoMultiplierExp:
            return data.parameterCache.at(parameterIdx).map([](auto p) { return float(p->lfo.multiplierExp); });
         case musicDevice::sound::ParameterAttr::LfoWaveform:
            return data.parameterCache.at(parameterIdx).map([](auto p){ return float(static_cast<int>(p->lfo.waveform)); });
      }
   return tl::unexpected(Error::indexOutOfRange);
}

Ret<float> KitComponent::getSDParameterValue(
    int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (sdVoiceRef.soundHandler)
   {
      const auto val = sdVoiceRef.soundHandler->getParameterValue(sdVoiceRef.sdVoiceIdx, parameterIdx,
                                               parameterAttr);
      if(val) return val.value();
      return tl::unexpected(Error::indexOutOfRange);
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Ret<float> KitComponent::fromNormalizedValue(
    int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (sdVoiceRef.soundHandler)
   {
      return sdVoiceRef.soundHandler->fromNormalizedValue(
          sdVoiceRef.sdVoiceIdx, parameterId, parameterAttr, percentageValue);
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Ret<const musicDevice::description::sound::Parameter*>
KitComponent::parameterDescription(int parameterIdx) const
{
   if (sdVoiceRef.soundHandler)
   {
      return &sdVoiceRef.soundHandler->parameterDescription(sdVoiceRef.sdVoiceIdx, parameterIdx);
   }
   return tl::unexpected(Error::noSoundDeviceCached);
}

Void KitComponent::clearModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr)
{
   return data.parameterCache.clearModifier(parameterIdx, parameterAttr);
}

Void KitComponent::applyModifier(std::size_t parameterIdx,
                              musicDevice::sound::ParameterAttr parameterAttr,
                              float destination, float intensity)
{
   return data.parameterCache.applyModifier(parameterIdx, parameterAttr, destination,
                                    intensity);
}
