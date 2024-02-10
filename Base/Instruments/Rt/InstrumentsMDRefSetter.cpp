#include "InstrumentsMDRefSetter.h"

using namespace base;
using namespace base::instruments::rt;

InstrumentsMDRefSetter::InstrumentsMDRefSetter(Data& rData) : m_rData(rData) {}

void InstrumentsMDRefSetter::fillReferencesKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   for (auto& kitInstrument : m_rData.kitInstruments)
   {
      kitInstrument.forEachComponent([&pMusicDevice](Component& component) {
         if (component.soundDeviceId() == pMusicDevice->deviceId())
         {
            component.setSoundDevicePtr(
                pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
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
      std::ranges::for_each(
          melodicInstrument.voices(), [&pMusicDevice](MelodicVoice& voice) {
             std::ranges::for_each(
                 voice.components, [&pMusicDevice](auto& component) {
                    if (component &&
                        (component->soundDeviceId() == pMusicDevice->deviceId()))
                    {
                       component->setSoundDevicePtr(
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
      it->forEachComponent([&isDeviceContained,
                            &pMusicDevice](Component& component) {
         if (component.pSoundDevice() == &pMusicDevice->soundHandler.value())
         {
            isDeviceContained = true;
            component.setSoundDevicePtr(nullptr);
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
