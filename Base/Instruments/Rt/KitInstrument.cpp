#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments::rt;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_voices(16)
{
}

void KitInstrument::noteOn(int note, float velocity, void* token) const
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOn(*vi, 64, velocity, token);
   }
}

void KitInstrument::noteOff(int note, float velocity, void* token) const
{
   auto vi = toVoiceIndex(note);
   if (vi)
   {
      noteOff(*vi, 64, velocity, token);
   }
}

void KitInstrument::noteOn(int voiceIdx, int note, float velocity,
                           void* token) const
{
   for (auto& component : m_voices[voiceIdx].components)
   {
      component.noteOn(note + m_voices[voiceIdx].noteOffset, velocity);
   }
   rtData->emitNoteOnPlayed(voiceIdx + 64, velocity, token);
}

void KitInstrument::noteOff(int voiceIdx, int note, float velocity,
                            void* token) const
{
   for (auto& component : m_voices[voiceIdx].components)
   {
      component.noteOff(note, velocity);
   }
   rtData->emitNoteOffPlayed(voiceIdx + 64, velocity, token);
}

void KitInstrument::incrementParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.incrementParameterValue(parameterIdx, parameterAttr, increment,
                                        incrementMode);
   });
}

void KitInstrument::incrementParameterValueEventBound(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.incrementParameterValueDontCache(parameterIdx, parameterAttr,
                                                 increment, incrementMode);
   });
}

std::optional<float> KitInstrument::getParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   std::optional<float> ret;
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      ret = component.getParameterValue(parameterIdx, parameterAttr);
   });
   return ret;
}

void KitInstrument::setParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.setParameterValue(parameterIdx, parameterAttr, value);
   });
}

void KitInstrument::setRelativeParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float relValue) const
{
   const auto actVal =
       getParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr);
   if (actVal)
   {
      withComponent(voiceIdx, componentIdx, [&](const Component& component) {
         component.setParameterValueDontCache(parameterIdx, parameterAttr,
                                              actVal.value() + relValue);
      });
   }
}

float KitInstrument::fromNormalizedValue(
    int voiceIdx, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   float ret = 0.0;
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      ret = component.fromNormalizedValue(parameterId, parameterAttr,
                                          percentageValue);
   });
   return ret;
}

void KitInstrument::clearModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.clearModifier(componentIdx, parameterAttr);
   });
}

void KitInstrument::applyModifier(
    int voiceIdx, int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float destination,
    float intensity) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.applyModifier(parameterIdx, parameterAttr, destination,
                              intensity);
   });
}

const base::musicDevice::description::sound::Parameter*
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   const base::musicDevice::description::sound::Parameter* ret{nullptr};
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      ret = component.parameterDescription(parameterIdx);
   });
   return ret;
}

std::optional<int> KitInstrument::toVoiceIndex(int note) const
{
   const int noteAdjusted = note - 64;
   if (0 <= noteAdjusted && noteAdjusted < m_voices.size())
   {
      return noteAdjusted;
   }
   return std::nullopt;
}

void KitInstrument::setVoiceNoteOffset(int voiceIdx, int offset)
{
   if(offset != m_voices[voiceIdx].noteOffset)
   {
      m_voices[voiceIdx].noteOffset = offset;
      emitVoiceNoteOffsetChanged(voiceIdx, offset);
   }
}
void KitInstrument::setComponentNoteOffset(int voiceIdx, int componentIdx, int offset) 
{
   if(offset != m_voices[voiceIdx].components[componentIdx].noteOffset())
   {
      m_voices[voiceIdx].components[componentIdx].setNoteOffset(offset);
      emitComponentNoteOffsetChanged(voiceIdx, componentIdx, offset);
   }
}

void KitInstrument::setVoiceAmp(int voiceIdx, float amp)
{
   if(amp != m_voices[voiceIdx].amp)
   {
      m_voices[voiceIdx].amp = amp;
      emitVoiceAmpChanged(voiceIdx, amp);
   }
}

void KitInstrument::setComponentAmp(int voiceIdx, int componentIdx, float amp)
{
   if(amp != m_voices[voiceIdx].components[componentIdx].amp())
   {
      m_voices[voiceIdx].components[componentIdx].setAmp(amp, 0);
      //TODO: really emit from here? emitComponentAmpChanged(voiceIdx, componentIdx, amp);
   }
}
