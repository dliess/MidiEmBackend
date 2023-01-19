#include "InstrumentsMDChangeHandler.h"

#include "InstrumentsData.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "MusicDeviceId.h"

using namespace base::instruments;

InstrumentsMDChangeHandler::InstrumentsMDChangeHandler(
    Data& rData,
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept :
    m_rData(rData)
{
   /*
   rFactoryDataHolder.onMusicDeviceAdded(
       [this](auto md) {
          add(std::move(md));
          m_rData.triggerChanged();
          // spdlog::info( "m_rData.triggerChanged();");
       });
   rFactoryDataHolder.musicDevices.onAboutToRemove(
       [this](std::shared_ptr<musicDevice::MusicDevice> ptr) {
          remove(std::move(ptr));
          m_rData.triggerChanged();
          // spdlog::info( "m_rData.triggerChanged();");
       });
   */
}
void InstrumentsMDChangeHandler::fillReferences(
    musicDevice::MusicDevice* pMusicDevice) noexcept
{
   fillReferencesKitInstruments(pMusicDevice);
   fillReferencesMelodicInstruments(pMusicDevice);
}

void InstrumentsMDChangeHandler::fillReferencesKitInstruments(
    musicDevice::MusicDevice* pMusicDevice) noexcept
{
   for (auto& kitInstrument : m_rData.kitInstruments)
   {
      kitInstrument.forEachVoice([&pMusicDevice](Voice& voice) {
         if (voice.soundDeviceId == pMusicDevice->deviceId())
         {
            voice.pSoundDevice = pMusicDevice->soundHandler
                                     ? &pMusicDevice->soundHandler.value()
                                     : nullptr;
         }
      });
   }
}

void InstrumentsMDChangeHandler::fillReferencesMelodicInstruments(
    musicDevice::MusicDevice* pMusicDevice) noexcept
{
   for (auto& melodicInstrument : m_rData.melodicInstruments)
   {
      std::for_each(
          melodicInstrument.voices().begin(), melodicInstrument.voices().end(),
          [&pMusicDevice](CompositeSound& compositeSound) {
             std::for_each(
                 compositeSound.voices.begin(), compositeSound.voices.end(),
                 [&pMusicDevice](Voice& voice) {
                    if (voice.soundDeviceId == pMusicDevice->deviceId())
                    {
                       voice.pSoundDevice =
                           pMusicDevice->soundHandler
                               ? &pMusicDevice->soundHandler.value()
                               : nullptr;
                    }
                 });
          });
   }
}

void InstrumentsMDChangeHandler::addDefaultInstrumentsFor(
    musicDevice::MusicDevice* pMusicDevice) noexcept
{
   switch (pMusicDevice->description()->soundSection->defaultInstrumentType)
   {
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::DrumKit:
      {
         addDefaultInstrumentsForDrumKit(pMusicDevice);
         break;
      }
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::InstrumentPerVoice:
      {
         addDefaultInstrumentsForInstrumentPerVoice(pMusicDevice);
         break;
      }
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::OnePolyphonicInstrument:
      {
         addDefaultInstrumentsForOnePolyphonicInstrument(pMusicDevice);
         break;
      }
      default:
      {
         spdlog::error("INTERNAL ERROR");
         break;
      }
   }
}

void InstrumentsMDChangeHandler::addDefaultInstrumentsForDrumKit(
    musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   KitInstrument kitInstrument(pMusicDevice->description()->productName);
   kitInstrument.markAsDefaultCreated();
   for (int voiceIndex = 0; voiceIndex < voiceDescr.size(); ++voiceIndex)
   {
      CompositeSound kompositeSound;
      kompositeSound.name = voiceDescr[voiceIndex].name;
      Voice voice;
      voice.soundDeviceId = pMusicDevice->deviceId();
      voice.pSoundDevice  = pMusicDevice->soundHandler
                                ? &pMusicDevice->soundHandler.value()
                                : nullptr;
      voice.voiceIndex    = voiceIndex;
      voice.noteOffset    = 0;
      kompositeSound.voices.push_back(voice);
      kitInstrument.addSound(voiceIndex, kompositeSound);
   }
   m_rData.kitInstruments.push_back(std::move(kitInstrument));
}

void InstrumentsMDChangeHandler::addDefaultInstrumentsForInstrumentPerVoice(
    musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   for (int voiceIndex = 0; voiceIndex < voiceDescr.size(); ++voiceIndex)
   {
      std::string name = pMusicDevice->description()->productName;
      if (voiceDescr.size() > 1)
      {
         name = name + " - " + std::to_string(voiceIndex + 1);
      }
      MelodicInstrument melodicInstrument(name);
      melodicInstrument.markAsDefaultCreated();
      CompositeSound compositeSound;
      compositeSound.name = voiceDescr[voiceIndex].name;
      Voice voice;
      voice.pSoundDevice  = pMusicDevice->soundHandler
                                ? &pMusicDevice->soundHandler.value()
                                : nullptr;
      voice.soundDeviceId = pMusicDevice->deviceId();
      voice.voiceIndex    = voiceIndex;
      voice.noteOffset    = 0;
      compositeSound.voices.push_back(voice);
      melodicInstrument.voices().push_back(std::move(compositeSound));
      m_rData.melodicInstruments.push_back(std::move(melodicInstrument));
   }
}

void InstrumentsMDChangeHandler::
    addDefaultInstrumentsForOnePolyphonicInstrument(
        musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   MelodicInstrument melodicInstrument(
       pMusicDevice->description()->productName);
   melodicInstrument.markAsDefaultCreated();
   for (int voiceIndex = 0; voiceIndex < voiceDescr.size(); ++voiceIndex)
   {
      CompositeSound compositeSound;
      compositeSound.name = voiceDescr[voiceIndex].name;
      Voice voice;
      voice.pSoundDevice  = pMusicDevice->soundHandler
                                ? &pMusicDevice->soundHandler.value()
                                : nullptr;
      voice.soundDeviceId = pMusicDevice->deviceId();
      voice.voiceIndex    = voiceIndex;
      voice.noteOffset    = 0;
      compositeSound.voices.push_back(voice);
      melodicInstrument.voices().push_back(std::move(compositeSound));
   }
   m_rData.melodicInstruments.push_back(std::move(melodicInstrument));
}

void InstrumentsMDChangeHandler::add(musicDevice::MusicDevice* pMusicDevice)
{
   assert(pMusicDevice);
   assert(pMusicDevice->description()->soundSection);
   fillReferences(pMusicDevice);
   addDefaultInstrumentsFor(pMusicDevice);
}

void InstrumentsMDChangeHandler::remove(musicDevice::MusicDevice* pMusicDevice)
{
   removeKitInstruments(pMusicDevice);
   removeMelodicInstruments(pMusicDevice);
}

void InstrumentsMDChangeHandler::removeKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   auto it = m_rData.kitInstruments.begin();
   while (it != m_rData.kitInstruments.end())
   {
      bool isDeviceContained{false};
      it->forEachVoice([&isDeviceContained, &pMusicDevice](Voice& voice) {
         if (voice.pSoundDevice == &pMusicDevice->soundHandler.value())
         {
            isDeviceContained  = true;
            voice.pSoundDevice = nullptr;
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

void InstrumentsMDChangeHandler::removeMelodicInstruments(
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
                              if (voice.pSoundDevice ==
                                  &pMusicDevice->soundHandler.value())
                              {
                                 isDeviceContained  = true;
                                 voice.pSoundDevice = nullptr;
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