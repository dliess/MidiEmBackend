#include "KitInstrument.h"

#include "ErrorHandling.h"
#include "Identifiable.h"
#include "MusicDeviceHolder.h"
#include "InstrumentParameterHandler.h"

using namespace base::instruments::rt;


KitInstrument::Voice::Component::Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                            const ParametersDescr& parametersDescr) noexcept :
    sdVoiceRef(soundDeviceId, sdVoiceIdx, pSoundDevice),
    data(parametersDescr)
{
}

KitInstrument::Voice::Component::Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                            const ParametersDescr& parametersDescr,
                            const ParameterData& paramData) noexcept :
    sdVoiceRef(soundDeviceId, sdVoiceIdx, pSoundDevice),
    data(parametersDescr, paramData)
{
}

KitInstrument::KitInstrument(util::Identifiable::UUIDView uuid) noexcept :
   Instrument(uuid),
   m_voices(NUM_VOICES)
{
}

Void KitInstrument::noteOn(int note, float velocity, void* token) 
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOn(*vi, 64, velocity, token);
      return Void{};
   }
   return tl::unexpected(Error::indexOutOfRange);
}

Void KitInstrument::noteOff(int note, float velocity, void* token)
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOff(*vi, 64, velocity, token);
      return Void{};
   }
   return tl::unexpected(Error::indexOutOfRange);
}

Void KitInstrument::noteOn(int voiceIdx, int note, float velocity,
                           void* token)
{
   return safe_at(m_voices, voiceIdx).map([&,this](auto voice) {
      for (auto& component : voice->components)
      {
         ParameterHandler ph(component.data, component.sdVoiceRef);
         if (component.sdVoiceRef.soundHandler)
         {
            if (component.sdVoiceRef.soundHandler->lastplayerId !=
                static_cast<const void*>(this))
            {
               ph.refreshParameters();
               component.sdVoiceRef.soundHandler->lastplayerId =
                   static_cast<const void*>(this);
            }
            component.sdVoiceRef.soundHandler->noteOn(component.sdVoiceRef.sdVoiceIdx, 
                                                      note + voice->noteOffset + component.data.noteOffset,
                                                      velocity);
         }
      }
      emitNoteOnPlayed(voiceIdx + 64, velocity, token);
   });
}

Void KitInstrument::noteOff(int voiceIdx, int note, float velocity,
                            void* token) 
{
   return safe_at(m_voices, voiceIdx).map([&,this](auto voice) {
      for (auto& component : voice->components)
      {
         if (component.sdVoiceRef.soundHandler)
         {
            component.sdVoiceRef.soundHandler->noteOff(component.sdVoiceRef.sdVoiceIdx, 
                                                       note + voice->noteOffset + component.data.noteOffset, 
                                                       velocity);
         }
      }
      emitNoteOffPlayed(voiceIdx + 64, velocity, token);
   });
}

Void KitInstrument::incrementParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return ParameterHandler(component->data, component->sdVoiceRef).
         incrementParameterValue(parameterIdx, parameterAttr, increment, incrementMode);
   });
}

Void KitInstrument::incrementParameterValueEventBound(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return ParameterHandler(component->data, component->sdVoiceRef).
         incrementParameterValueDontCache(parameterIdx, parameterAttr, increment, incrementMode);
   });
}

Ret<float> KitInstrument::getParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->data.parameterCache.getParameterValue(parameterIdx, parameterAttr);
   });
}

Void KitInstrument::setParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return ParameterHandler(component->data, component->sdVoiceRef).
         setParameterValue(parameterIdx, parameterAttr, value);
   });
}

Void KitInstrument::setRelativeParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float relValue) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->data.parameterCache.getParameterValue(parameterIdx, parameterAttr).and_then([&](float actVal) {
          return ParameterHandler(component->data, component->sdVoiceRef).
             setParameterValueDontCache(parameterIdx, parameterAttr, actVal + relValue);
          });
   });
}

Ret<float> KitInstrument::fromNormalizedValue(
    int voiceIdx, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) -> Ret<float> {
      if (component->sdVoiceRef.soundHandler)
      {
         return component->sdVoiceRef.soundHandler->fromNormalizedValue(
             component->sdVoiceRef.sdVoiceIdx, parameterId, parameterAttr, percentageValue);
      }
      return tl::unexpected(Error::noSoundDeviceCached);
   });
}

Void KitInstrument::clearModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->data.parameterCache.clearModifier(parameterIdx, parameterAttr);
   });
}

Void KitInstrument::applyModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float destination,
    float intensity)
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->data.parameterCache.applyModifier(parameterIdx, parameterAttr, destination,
                                    intensity);
   });
}

Ret<const base::musicDevice::description::sound::Parameter*>
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) 
                                                        -> Ret<const base::musicDevice::description::sound::Parameter*> {
      if (component->sdVoiceRef.soundHandler)
      {
         return &component->sdVoiceRef.soundHandler->parameterDescription(component->sdVoiceRef.sdVoiceIdx, parameterIdx);
      }
      return tl::unexpected(Error::noSoundDeviceCached);
   });
}

Ret<int> KitInstrument::toVoiceIndex(int note) const
{
   const int noteAdjusted = note - 64;
   if (0 <= noteAdjusted && noteAdjusted < m_voices.size())
   {
      return noteAdjusted;
   }
   return tl::unexpected(Error::indexOutOfRange);
}

Void KitInstrument::setVoiceNoteOffset(int voiceIdx, int offset)
{
   return safe_at(m_voices, voiceIdx).map([&,this](auto voice) {
      if(voice->noteOffset != offset)
      {
         voice->noteOffset = offset;
      }
   });
}

Void KitInstrument::setComponentNoteOffset(int voiceIdx, int componentIdx, int offset) 
{
   return getComponent(voiceIdx, componentIdx).map([&,this](auto component) {
      if(component->data.noteOffset != offset)
      {
         component->data.noteOffset = offset;
      }
   });
}

Void KitInstrument::setVoiceAmp(int voiceIdx, float amp)
{
   return safe_at(m_voices, voiceIdx).map([&,this](auto voice) {
      if(amp != voice->amp)
      {
         voice->amp = amp;
      }
   });
}

Void KitInstrument::setComponentAmp(int voiceIdx, int componentIdx, float amp)
{
   return getComponent(voiceIdx, componentIdx).map([&,this](auto component) {
      const float goalAmp = amp * m_voices[voiceIdx].amp;
      if(goalAmp != component->data.amp)
      {
         component->data.amp = goalAmp;
      }
   });
}

void KitInstrument::updateParameterUI() 
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components)
      {
         component.data.parameterCache.updateParameterUI();
      }
   }
}

Ret<KitInstrument::Voice::Component*> KitInstrument::getComponent(int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
       [&](Voice* voice) -> Ret<Voice::Component*>{
          return safe_at(voice->components, componentIdx);
       });
}

Ret<const KitInstrument::Voice::Component*> KitInstrument::getComponent(int voiceIdx, int componentIdx) const noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
       [&](const Voice* voice) -> Ret<const Voice::Component*>{
          return safe_at(voice->components, componentIdx);
       });
}

