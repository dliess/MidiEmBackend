#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments::loader;

Ret<KitComponent*> KitInstrument::getComponent(int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
      [componentIdx](auto voice) -> Ret<KitComponent*> {
         return safe_at(voice->components, componentIdx);
      });
}
Ret<const KitComponent*> KitInstrument::getConstComponent(int voiceIdx, int componentIdx) const noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
      [componentIdx](auto voice) -> Ret<const KitComponent*> {
         return safe_at(voice->components, componentIdx);
      });
}

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_voices(MAX_VOICES)
{
}

Ret<float> KitInstrument::getParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return getConstComponent(voiceIdx, componentIdx).and_then(
      [parameterIdx, parameterAttr](auto component) {
         return component->getParameterValue(parameterIdx, parameterAttr);
   });
}

Void KitInstrument::setParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) 
{
   return getComponent(voiceIdx, componentIdx).and_then(
      [parameterIdx, parameterAttr, value](auto component) {
         return component->setParameterValue(parameterIdx, parameterAttr, value);
      });
}

Ret<const base::musicDevice::description::sound::Parameter*>
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   return getConstComponent(voiceIdx, componentIdx).and_then(
      [parameterIdx](auto component) {
         return component->parameterDescription(parameterIdx);
      });
}

Void KitInstrument::setVoiceNoteOffset(int voiceIdx, int offset)
{
   return safe_at(m_voices, voiceIdx).map(
      [this,voiceIdx, offset](auto voice) {
         if(voice->noteOffset != offset)
         {
            voice->noteOffset = offset;
         }
      });
}
Void KitInstrument::setComponentNoteOffset(int voiceIdx, int componentIdx, int offset) 
{
   return getComponent(voiceIdx, componentIdx).map(
      [this,voiceIdx, componentIdx, offset](auto component) {
         if(offset != component->noteOffset())
         {
            component->setNoteOffset(offset);
         }
      });
}

Void KitInstrument::setVoiceAmp(int voiceIdx, float amp)
{
   return safe_at(m_voices, voiceIdx).map(
      [this,voiceIdx, amp](auto voice) {
         if(amp != voice->amp)
         {
            voice->amp = amp;
         }
      });
}

Void KitInstrument::setComponentAmp(int voiceIdx, int componentIdx, float amp)
{
   return getComponent(voiceIdx, componentIdx).map(
      [this,voiceIdx, componentIdx, amp](auto component) {
         if(amp != component->amp())
         {
             component->setAmp(amp); 
        }
      });
}
