#include "MelodicInstrumentCopyer.h"
#include "Rt/MelodicInstrument.h"

using namespace base::instruments;


rt::MelodicInstrument MelodicInstrumentCopyer::copy(const base::musicDevice::MusicDeviceContainer& mdContainer,
                                                    const loader::MelodicInstrument& src) noexcept
{
   rt::MelodicInstrument dst(src.id());
   for (const auto& srcVoice : src.m_voices)
   {
      rt::MelodicInstrument::Voice dstVoice;
      for (int i = 0; i < srcVoice.components.size(); ++i)
      {
         const auto& srcComponent = srcVoice.components[i];
         if(srcComponent)
         {
            musicDevice::sound::SoundHandler* sh = nullptr;
            auto mdIt = mdContainer.findByDeviceId(srcComponent->soundDeviceId);
            if (mdIt != mdContainer.end() && mdIt->second->soundHandler)
            {
               sh = &mdIt->second->soundHandler.value();
            }
            dstVoice[i].emplace(srcComponent->soundDeviceId,
                                srcComponent->sdVoiceIdx,
                                sh);
         }
      }
      dst.m_voices.push_back(dstVoice);
   }
   for(int i = 0; i < src.m_parameters.size(); ++i)
   {
      const auto& srcParam = src.m_parameters[i];
      if(srcParam)
      {
         dst.m_engines[i].emplace(*srcParam->parametersDescr,
                                 srcParam->deviceParameters);
      }
   }
   return dst;
}
