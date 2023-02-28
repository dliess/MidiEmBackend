#include "InstrumentsMDRefSetter.h"

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

void InstrumentsMDRefSetter::removeKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   auto it = m_rData.kitInstruments.begin();
   while (it != m_rData.kitInstruments.end())
   {
      bool isDeviceContained{false};
      it->forEachVoice([&isDeviceContained, &pMusicDevice](Voice& voice) {
         if (voice.pSoundDevice() == &pMusicDevice->soundHandler.value())
         {
            isDeviceContained = true;
            voice.setSoundDevicePtr(nullptr);
         }
      });
      if (isDeviceContained && it->isDefaultCreated())
      {
         spdlog::info("Erasing");
         it = m_rData.kitInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
}

void InstrumentsMDRefSetter::removeMelodicInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   auto it = m_rData.melodicInstruments.begin();
   while (it != m_rData.melodicInstruments.end())
   {
      bool isDeviceContained{false};
      std::for_each(
          it->voices().begin(), it->voices().end(),
          [&isDeviceContained, &pMusicDevice](CompositeSound& compositeSound) {
             std::for_each(compositeSound.voices.begin(),
                           compositeSound.voices.end(),
                           [&isDeviceContained, &pMusicDevice](Voice& voice) {
                              if (voice.pSoundDevice() ==
                                  &pMusicDevice->soundHandler.value())
                              {
                                 isDeviceContained = true;
                                 voice.setSoundDevicePtr(nullptr);
                              }
                           });
          });
      if (isDeviceContained && it->isDefaultCreated())
      {
         it = m_rData.melodicInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
}
