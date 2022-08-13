#include "InstrumentsFactory.h"

#include "Instruments.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceId.h"

using namespace base::instruments;

InstrumentsFactory::InstrumentsFactory(
    Instruments& rInstruments, musicDevice::Holder& rMusicDeviceHolder) noexcept
    :
    m_rInstruments(rInstruments)
{
   rMusicDeviceHolder.musicDevices.onAdded(
       [this,
        &rMusicDeviceHolder](std::shared_ptr<musicDevice::MusicDevice> ptr) {
          add(std::move(ptr));
          m_rInstruments.triggerChanged();
          // spdlog::info( "m_rInstruments.triggerChanged();");
       });
   rMusicDeviceHolder.musicDevices.onAboutToRemove(
       [this,
        &rMusicDeviceHolder](std::shared_ptr<musicDevice::MusicDevice> ptr) {
          remove(std::move(ptr));
          m_rInstruments.triggerChanged();
          // spdlog::info( "m_rInstruments.triggerChanged();");
       });
}

void InstrumentsFactory::fillReferencesInOtherInstruments(
    const std::shared_ptr<musicDevice::MusicDevice>& pMusicDevice) noexcept
{
   for (auto& kitInstrument : m_rInstruments.data.kitInstruments)
   {
      kitInstrument->forEachVoice([&pMusicDevice](Voice& voice) {
         if (voice.soundDeviceId == pMusicDevice->deviceId())
         {
            voice.pSoundDevice = pMusicDevice->soundHandler
                                          ? &pMusicDevice->soundHandler.value()
                                          : nullptr;
         }
      });
   }
   for (auto& melodicInstrument : m_rInstruments.data.melodicInstruments)
   {
      melodicInstrument->forEachVoice(
          [&pMusicDevice](MelodicInstrumentVoice& voice) {
             if (voice.soundDeviceId == pMusicDevice->deviceId())
             {
                voice.pSoundDevice = pMusicDevice;
             }
          });
   }
}

void InstrumentsFactory::addDefaultInstrumentsFor(
    const std::shared_ptr<musicDevice::MusicDevice>& pMusicDevice) noexcept
{
   switch (pMusicDevice->description()->soundSection->defaultInstrumentType)
   {
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::DrumKit:
      {
         auto kitInstrument = std::make_shared<KitInstrument>(
             pMusicDevice->description()->productName);
         kitInstrument->markAsDefaultCreated();
         for (int i = 0;
              i < pMusicDevice->description()->soundSection->voices.size(); ++i)
         {
            CompositeSound kompositeSound;
            kompositeSound.name =
                pMusicDevice->description()->soundSection->voices[i].name;
            Voice voice;
            voice.soundDeviceId = pMusicDevice->deviceId();
            voice.pSoundDevice  = pMusicDevice->soundHandler
                                           ? &pMusicDevice->soundHandler.value()
                                           : nullptr;
            voice.voiceIndex    = i;
            voice.noteOffset    = 0;
            kompositeSound.voices.push_back(voice);
            kitInstrument->addSound(kompositeSound);
         }
         m_rInstruments.data.kitInstruments.push_back(std::move(kitInstrument));
         break;
      }
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::InstrumentPerVoice:
      {
         for (int i = 0;
              i < pMusicDevice->description()->soundSection->voices.size(); ++i)
         {
            std::string name = pMusicDevice->description()->productName;
            if (pMusicDevice->description()->soundSection->voices.size() > 1)
               name = name + " - " + std::to_string(i + 1);
            auto melodicInstrument = std::make_shared<MelodicInstrument>(name);
            melodicInstrument->markAsDefaultCreated();
            MelodicInstrumentVoice voice;
            voice.pSoundDevice  = pMusicDevice;
            voice.soundDeviceId = pMusicDevice->deviceId();
            voice.voiceIndex    = i;
            melodicInstrument->addVoice(std::move(voice));
            m_rInstruments.data.melodicInstruments.push_back(
                std::move(melodicInstrument));
         }
         break;
      }
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::OnePolyphonicInstrument:
      {
         auto melodicInstrument = std::make_shared<MelodicInstrument>(
             pMusicDevice->description()->productName);
         melodicInstrument->markAsDefaultCreated();
         for (int i = 0;
              i < pMusicDevice->description()->soundSection->voices.size(); ++i)
         {
            MelodicInstrumentVoice voice;
            voice.pSoundDevice  = pMusicDevice;
            voice.soundDeviceId = pMusicDevice->deviceId();
            voice.voiceIndex    = i;
            melodicInstrument->addVoice(std::move(voice));
         }
         m_rInstruments.data.melodicInstruments.push_back(
             std::move(melodicInstrument));
         break;
      }
      default:
      {
         spdlog::error("INTERNAL ERROR");
         break;
      }
   }
}

void InstrumentsFactory::add(
    std::shared_ptr<musicDevice::MusicDevice> pMusicDevice)
{
   assert(pMusicDevice);
   if (!pMusicDevice->description()->soundSection)
   {
      return;
   }
   fillReferencesInOtherInstruments(pMusicDevice);
   addDefaultInstrumentsFor(pMusicDevice);
}

void InstrumentsFactory::remove(
    std::shared_ptr<musicDevice::MusicDevice> pMusicDevice)
{
   {
      auto it = m_rInstruments.data.kitInstruments.begin();
      while (it != m_rInstruments.data.kitInstruments.end())
      {
         bool isDeviceContained{false};
         (*it)->forEachVoice(
             [&isDeviceContained, &pMusicDevice](Voice& voice) {
                if (voice.pSoundDevice == &pMusicDevice->soundHandler.value())
                {
                   isDeviceContained       = true;
                   voice.pSoundDevice = nullptr;
                }
             });
         if (isDeviceContained && (*it)->isDefaultCreated())
         {
            spdlog::info("Erasing");
            it = m_rInstruments.data.kitInstruments.erase(it);
         }
         else
         {
            ++it;
         }
      }
   }
   {
      auto it = m_rInstruments.data.melodicInstruments.begin();
      while (it != m_rInstruments.data.melodicInstruments.end())
      {
         bool isDeviceContained{false};
         (*it)->forEachVoice([&isDeviceContained,
                              &pMusicDevice](MelodicInstrumentVoice& voice) {
            if (voice.pSoundDevice == pMusicDevice)
            {
               isDeviceContained = true;
               voice.pSoundDevice.reset();
            }
         });
         if (isDeviceContained && (*it)->isDefaultCreated())
         {
            it = m_rInstruments.data.melodicInstruments.erase(it);
         }
         else
         {
            ++it;
         }
      }
   }
}
