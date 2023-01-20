#include "InstrumentsMDChangeHandler.h"

#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"

using namespace base::instruments;

namespace detail
{
void fillReferencesKitInstruments(
    Data& rData, musicDevice::MusicDevice* pMusicDevice) noexcept
{
   for (auto& kitInstrument : rData.kitInstruments)
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

void fillReferencesMelodicInstruments(
    Data& rData, musicDevice::MusicDevice* pMusicDevice) noexcept
{
   for (auto& melodicInstrument : rData.melodicInstruments)
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

void removeKitInstruments(Data rData, musicDevice::MusicDevice* pMusicDevice)
{
   auto it = rData.kitInstruments.begin();
   while (it != rData.kitInstruments.end())
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
         it = rData.kitInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
}

void removeMelodicInstruments(Data& rData,
                              musicDevice::MusicDevice* pMusicDevice)
{
   auto it = rData.melodicInstruments.begin();
   while (it != rData.melodicInstruments.end())
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
         it = rData.melodicInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
}

}   // namespace detail

InstrumentsMDChangeHandler::InstrumentsMDChangeHandler(
    util::DoubleBuffer<Data>& rDoubleBufferedData) noexcept :
    m_rDoubleBufferedData(rDoubleBufferedData)
{
}

void InstrumentsMDChangeHandler::fillReferences(
    musicDevice::MusicDevice* pMusicDevice) noexcept
{
   m_rDoubleBufferedData.withNonRtLocked([this, pMusicDevice](auto& nonRtData) {
      detail::fillReferencesKitInstruments(nonRtData, pMusicDevice);
      detail::fillReferencesMelodicInstruments(nonRtData, pMusicDevice);
   });
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
   m_rDoubleBufferedData.withNonRtLocked(
       [this, &kitInstrument, &melodicRtData](auto& nonRtData) {
          nonRtData.kitInstruments.push_back(kitInstrument);
       });
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
      MelodicInstrument melodicInstrument(
          name, std::make_shared<MelodicInstrument::RtData>());
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

      m_rDoubleBufferedData.withNonRtLocked(
          [this, &melodicInstrument](auto& nonRtData) {
             nonRtData.melodicInstruments.push_back(melodicInstrument);
          });
   }
}

void InstrumentsMDChangeHandler::
    addDefaultInstrumentsForOnePolyphonicInstrument(
        musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   MelodicInstrument melodicInstrument(
       pMusicDevice->description()->productName,
       std::make_shared<MelodicInstrument::RtData>());
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
   m_rDoubleBufferedData.withNonRtLocked(
       [this, &melodicInstrument](auto& nonRtData) {
          nonRtData.melodicInstruments.push_back(melodicInstrument);
       });
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
   m_rDoubleBufferedData.withNonRtLocked([this, pMusicDevice](auto& nonRtData) {
      detail::removeKitInstruments(nonRtData, pMusicDevice);
      detail::removeMelodicInstruments(nonRtData, pMusicDevice);
   });
}