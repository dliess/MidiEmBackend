#include "KitInstrument.h"

#include "ErrorHandling.h"
#include "Identifiable.h"
#include "MusicDeviceHolder.h"
#include "InstrumentParameterHandler.h"

using namespace base::instruments::rt;

KitInstrument::KitInstrument(util::Identifiable::UUIDView uuid, std::string_view name) noexcept :
   Instrument(uuid),
   m_name(name), 
   m_voices(NUM_VOICES)
{
}

void KitInstrument::noteOn(int note, float velocity, void* token) 
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOn(*vi, 64, velocity, token);
   }
}

void KitInstrument::noteOff(int note, float velocity, void* token)
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOff(*vi, 64, velocity, token);
   }
}

void KitInstrument::noteOn(int voiceIdx, int note, float velocity,
                           void* token)
{
   for (auto& component : m_voices[voiceIdx].components)
   {
      ParameterHandler ph(component.data, component.sdVoiceRef);
      component.noteOn(note + m_voices[voiceIdx].noteOffset, velocity, [&ph] {
         ph.refreshParameters();
      });
   }
   emitNoteOnPlayed(voiceIdx + 64, velocity, token);
}

void KitInstrument::noteOff(int voiceIdx, int note, float velocity,
                            void* token) 
{
   for (auto& component : m_voices[voiceIdx].components)
   {
      component.noteOff(note, velocity);
   }
   emitNoteOffPlayed(voiceIdx + 64, velocity, token);
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
      return component->getParameterValue(parameterIdx, parameterAttr);
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
      return component->getParameterValue(parameterIdx, parameterAttr).and_then([&](float actVal) {
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
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->fromNormalizedValue(parameterId, parameterAttr,
                                        percentageValue);
   });
}

Void KitInstrument::clearModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) 
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->clearModifier(parameterIdx, parameterAttr);
   });
}

Void KitInstrument::applyModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float destination,
    float intensity)
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->applyModifier(parameterIdx, parameterAttr, destination,
                              intensity);
   });
}

Ret<const base::musicDevice::description::sound::Parameter*>
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   return getComponent(voiceIdx, componentIdx).and_then([&](auto component) {
      return component->parameterDescription(parameterIdx);
   });
}

std::string KitInstrument::name() const noexcept { return m_name; }

void KitInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
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
      if(component->noteOffset() != offset)
      {
         component->setNoteOffset(offset);
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
      if(amp != component->amp())
      {
         component->setAmp(amp, 0);
      }
   });
}

void KitInstrument::updateParameterUI() 
{
   for (auto& sound : m_voices)
   {
      for (auto& component : sound.components)
      {
         component.updateParameterUI();
      }
   }
}

Ret<KitComponent*> KitInstrument::getComponent(int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
       [&](KitVoice* voice) -> Ret<KitComponent*>{
          return safe_at(voice->components, componentIdx);
       });
}

Ret<const KitComponent*> KitInstrument::getComponent(int voiceIdx, int componentIdx) const noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
       [&](const KitVoice* voice) -> Ret<const KitComponent*>{
          return safe_at(voice->components, componentIdx);
       });
}

