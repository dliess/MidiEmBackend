#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments::loader;

Ret<KitInstrument::Component*> KitInstrument::getComponent(int voiceIdx, int componentIdx) noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
      [componentIdx](auto voice) -> Ret<KitInstrument::Component*> {
         return safe_at(voice->components, componentIdx);
      });
}
Ret<const KitInstrument::Component*> KitInstrument::getConstComponent(int voiceIdx, int componentIdx) const noexcept
{
   return safe_at(m_voices, voiceIdx).and_then(
      [componentIdx](auto voice) -> Ret<const KitInstrument::Component*> {
         return safe_at(voice->components, componentIdx);
      });
}

KitInstrument::KitInstrument(std::string name) noexcept :
    Instrument(std::move(name)), m_voices(MAX_VOICES)
{
}

// Todo: maybe remove
// Ret<float> KitInstrument::getParameterValue(
//     int voiceIdx, int componentIdx, int parameterIdx,
//     musicDevice::sound::ParameterAttr parameterAttr) const
// {
//    return getConstComponent(voiceIdx, componentIdx).and_then(
//       [parameterIdx, parameterAttr](auto component) {
//          return safe_at(component->parameterData, parameterIdx).map(
//             [parameterAttr](auto p) -> float{ 
//                return getParameterData(*p, parameterAttr);
//             });
//    });
// }

Void KitInstrument::setParameterValue(
    int voiceIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) 
{
   return getComponent(voiceIdx, componentIdx).and_then(
      [parameterIdx, parameterAttr, value](auto component) {
         return safe_at(component->parameterData, parameterIdx).map(
            [parameterAttr, value](auto p) -> void {
               setParameterData(*p, parameterAttr, value);
            });
      });
}

Ret<const base::musicDevice::description::sound::Parameter*>
KitInstrument::parameterDescription(int voiceIdx, int componentIdx,
                                    int parameterIdx) const
{
   return getConstComponent(voiceIdx, componentIdx).and_then(
      [parameterIdx](auto component) {
         return safe_at(*component->paramDescr, parameterIdx);
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
         if(offset != component->noteOffset)
         {
            component->noteOffset = offset;
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
         if(amp != component->amp)
         {
             component->amp = amp;
        }
      });
}
