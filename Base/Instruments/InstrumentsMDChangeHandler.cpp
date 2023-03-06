#include "InstrumentsMDChangeHandler.h"

#include "InstrumentVoiceFactory.h"
#include "Instruments.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"

using namespace base;
using namespace base::instruments;

InstrumentsMDChangeHandler::InstrumentsMDChangeHandler(
    Instruments& rInstruments) noexcept :
    m_rInstruments(rInstruments)
{
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
      auto paramCache = createParameterCache(pMusicDevice->description().get(),
                                             pMusicDevice->id(), voiceIndex);
      if (!paramCache)
      {
         spdlog::error("Could not create parameter cache");
         continue;
      }
      CompositeSound kompositeSound(voiceDescr[voiceIndex].name);
      kompositeSound.voices.emplace_back(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          paramCache, pMusicDevice->deviceId(), voiceIndex, 0);
      kitInstrument.addSound(voiceIndex, kompositeSound);
   }
   m_rInstruments.insertKitInstrument(kitInstrument);
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
      CompositeSound compositeSound(voiceDescr[voiceIndex].name);
      auto paramCache = createParameterCache(pMusicDevice->description().get(),
                                             pMusicDevice->id(), voiceIndex);
      if (!paramCache)
      {
         spdlog::error("Could not create parameter cache");
         continue;
      }
      compositeSound.voices.emplace_back(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          paramCache, pMusicDevice->deviceId(), voiceIndex, 0);
      melodicInstrument.voices().push_back(std::move(compositeSound));

      m_rInstruments.insertMelodicInstrument(melodicInstrument);
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
      auto paramCache = createParameterCache(pMusicDevice->description().get(),
                                             pMusicDevice->id(), voiceIndex);
      if (!paramCache)
      {
         spdlog::error("Could not create parameter cache");
         continue;
      }
      CompositeSound compositeSound(voiceDescr[voiceIndex].name);
      compositeSound.voices.emplace_back(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          paramCache, pMusicDevice->deviceId(), voiceIndex, 0);
      melodicInstrument.voices().push_back(std::move(compositeSound));
   }
   m_rInstruments.insertMelodicInstrument(melodicInstrument);
}

void InstrumentsMDChangeHandler::add(musicDevice::MusicDevice* pMusicDevice)
{
   assert(pMusicDevice);
   assert(pMusicDevice->description()->soundSection);
   m_rInstruments.fillReferencesToMD(pMusicDevice);
   addDefaultInstrumentsFor(pMusicDevice);
}

void InstrumentsMDChangeHandler::remove(musicDevice::MusicDevice* pMusicDevice)
{
   assert(pMusicDevice);
   assert(pMusicDevice->description()->soundSection);
   m_rInstruments.removeReferencesToMD(pMusicDevice);
}