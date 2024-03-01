#include "KitInstrumentCopyer.h"
#include "Rt/KitInstrument.h"

using namespace base::instruments;


rt::KitInstrument KitInstrumentCopyer::copy(const loader::KitInstrument& src) noexcept
{
   rt::KitInstrument dst(src.id());
   // for (const auto& srcVoice : src.m_voices)
   // {
   //    rt::KitVoice voice;
   //    for (const auto& srcComponent : srcVoice.components)
   //    {
   //       rt::KitComponent component(srcComponent.m_pSoundDevice, 
   //                                  srcComponent.m_soundDeviceId, 
   //                                  srcComponent.m_sdVoiceIdx, 
   //                                  srcComponent.m_noteOffset,
   //                                  srcComponent.m_amp);
   //       for(int parameterIdx = 0, parameterIdx < srcComponent.data.parameterCache.data_.size(), ++parameterIdx)
   //       {
   //          component.data.parameterCache.data_[parameterIdx] = srcComponent.data.parameterCache.data_[parameterIdx];
   //       }
   //       voice.components.push_back(component);
   //    }
   //    voice.noteOffset = srcVoice.noteOffset;
   //    voice.amp = srcVoice.amp;
   //    dst.m_voices.push_back(voice);
   // }
   return dst;
}



