#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments::loader;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_voices(16)
{
}

Ret<float> KitInstrument::getParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return safe_at(m_voices, voiceIdx).and_then(
      [componentIdx, parameterIdx, parameterAttr](auto voice) -> Ret<float> {
         return safe_at(voice.components, componentIdx).map(
            [parameterIdx, parameterAttr](auto component) -> float {
               return component->getParameterValue(parameterIdx, parameterAttr).value_or(0.0f);
            });
      });
      });
   std::optional<float> ret;
   withConstComponent(voiceIdx, componentIdx, [&](const Component& component) {
      ret = component.getParameterValue(parameterIdx, parameterAttr);
   });
   return ret;
}

void KitInstrument::setParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) 
{
   withComponent(voiceIdx, componentIdx, [&](Component& component) {
      component.setParameterValue(parameterIdx, parameterAttr, value);
   });
}

const base::musicDevice::description::sound::Parameter*
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   const base::musicDevice::description::sound::Parameter* ret{nullptr};
   withConstComponent(voiceIdx, componentIdx, [&](const Component& component) {
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
