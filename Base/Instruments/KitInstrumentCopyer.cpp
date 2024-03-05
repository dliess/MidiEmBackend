#include "KitInstrumentCopyer.h"
#include "MusicDeviceContainer.h"
#include "Rt/KitInstrument.h"

using namespace base::instruments;


rt::KitInstrument KitInstrumentCopyer::copy(const base::musicDevice::MusicDeviceContainer& mdContainer, 
                                            const loader::KitInstrument& src) noexcept
{
   rt::KitInstrument dst(src.id());
   for (const auto& srcVoice : src.m_voices)
   {
      rt::KitInstrument::Voice voice;
      for (const auto& srcComponent : srcVoice.components)
      {
         musicDevice::sound::SoundHandler* sh = nullptr;
         auto mdIt = mdContainer.findByDeviceId(srcComponent.soundDeviceId);
         if (mdIt != mdContainer.end() && mdIt->second->soundHandler)
         {
            sh = &mdIt->second->soundHandler.value();
         }
         rt::KitInstrument::Voice::Component component(sh, 
                                                       srcComponent.soundDeviceId, 
                                                       srcComponent.sdVoiceIdx, 
                                                       *srcComponent.paramDescr,
                                                       srcComponent.parameterData);
         component.data.noteOffset = srcComponent.noteOffset;
         component.data.amp = srcComponent.amp;
         voice.components.push_back(component);
      }
      voice.noteOffset = srcVoice.noteOffset;
      voice.amp = srcVoice.amp;
      dst.m_voices.push_back(voice);
   }
   return dst;
}



