#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

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
      component.noteOn(note, velocity);
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
    bool roundRobin) const
{
   withComponent(voiceIdx, componentIdx, [&](const Component& component) {
      component.incrementParameterValue(parameterIdx, parameterAttr, increment,
                                        roundRobin);
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
         component.setParameterValueDontCache(parameterIdx, parameterAttr, actVal.value() + relValue);
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