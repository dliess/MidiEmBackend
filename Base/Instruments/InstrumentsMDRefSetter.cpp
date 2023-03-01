#include "InstrumentsMDRefSetter.h"

using namespace base;
using namespace base::instruments;

InstrumentsMDRefSetter::InstrumentsMDRefSetter(Data& rData) : m_rData(rData) {}

void InstrumentsMDRefSetter::fillReferencesKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   for (auto& kitInstrument : m_rData.kitInstruments)
   {
      kitInstrument.forEachVoice([&pMusicDevice](Voice& voice) {
         if (voice.soundDeviceId() == pMusicDevice->deviceId())
         {
            voice.setSoundDevicePtr(pMusicDevice->soundHandler
                                        ? &pMusicDevice->soundHandler.value()
                                        : nullptr);
         }
      });
   }
}

void InstrumentsMDRefSetter::fillReferencesMelodicInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   for (auto& melodicInstrument : m_rData.melodicInstruments)
   {
      std::for_each(
          melodicInstrument.voices().begin(), melodicInstrument.voices().end(),
          [&pMusicDevice](CompositeSound& compositeSound) {
             std::for_each(
                 compositeSound.voices.begin(), compositeSound.voices.end(),
                 [&pMusicDevice](Voice& voice) {
                    if (voice.soundDeviceId() == pMusicDevice->deviceId())
                    {
                       voice.setSoundDevicePtr(
                           pMusicDevice->soundHandler
                               ? &pMusicDevice->soundHandler.value()
                               : nullptr);
                    }
                 });
          });
   }
}

namespace detail
{
template <class InstrumentContainer>
void removeReference(InstrumentContainer&& instrumentContainer,
                     musicDevice::MusicDevice* pMusicDevice)
{
   auto it = instrumentContainer.begin();
   while (it != instrumentContainer.end())
   {
      bool isDeviceContained{false};
      it->forEachVoice([&isDeviceContained, &pMusicDevice](Voice& voice) {
         if (voice.pSoundDevice() == &pMusicDevice->soundHandler.value())
         {
            isDeviceContained = true;
            voice.setSoundDevicePtr(nullptr);
         }
      });
      if (isDeviceContained && it->isDefaultCreated() && it->refCount() == 0)
      {
         it = instrumentContainer.erase(it);
      }
      else
      {
         ++it;
      }
   }
}
}   // namespace detail

void InstrumentsMDRefSetter::removeKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   detail::removeReference(m_rData.kitInstruments, pMusicDevice);
}

void InstrumentsMDRefSetter::removeMelodicInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   detail::removeReference(m_rData.melodicInstruments, pMusicDevice);
}
