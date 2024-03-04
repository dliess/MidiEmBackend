#include "InstrumentsMDChangeHandler.h"

// #include "InstrumentComponentParameterCacheCreator.h"
#include "Instruments.h"
#include "Loader/KitInstrumentModifier.h"
#include "Loader/MelodicInstrumentModifier.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"

using namespace base;
using namespace base::instruments;
using namespace base::instruments::loader;

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
   for (int voiceIdx = 0; voiceIdx < voiceDescr.size(); ++voiceIdx)
   {
      KitInstrument::Voice voice(voiceDescr[voiceIdx].name);
      voice.components.emplace_back(
          pMusicDevice->description()->soundSection->engineBase(voiceIdx)->parameters,
          pMusicDevice->deviceId(), voiceIdx);
      loader::KitInstrumentModifier(kitInstrument).addVoice(voiceIdx, voice);
   }
   if (!m_rInstruments.hasSameInstrument(kitInstrument))
   {
      m_rInstruments.insertKitInstrument(kitInstrument);
   }
}

void InstrumentsMDChangeHandler::addDefaultInstrumentsForInstrumentPerVoice(
    musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   for (int sdvoiceIdx = 0; sdvoiceIdx < voiceDescr.size(); ++sdvoiceIdx)
   {
      std::string name = pMusicDevice->description()->productName;
      if (voiceDescr.size() > 1)
      {
         name = name + " - " + std::to_string(sdvoiceIdx + 1);
      }
      MelodicInstrument melodicInstrument(name);
      MelodicInstrumentModifier(melodicInstrument).createNewVoiceInMelodicInstrument(
          pMusicDevice, sdvoiceIdx);
      melodicInstrument.markAsDefaultCreated();

      if (!m_rInstruments.hasSameInstrument(melodicInstrument))
      {
         m_rInstruments.insertMelodicInstrument(melodicInstrument);
      }
   }
}

void InstrumentsMDChangeHandler::
    addDefaultInstrumentsForOnePolyphonicInstrument(
        musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   MelodicInstrument melodicInstrument(pMusicDevice->description()->productName);
   for (int sdvoiceIdx = 0; sdvoiceIdx < voiceDescr.size(); ++sdvoiceIdx)
   {
      MelodicInstrumentModifier(melodicInstrument).createNewVoiceInMelodicInstrument(
          pMusicDevice, sdvoiceIdx);
   }
   melodicInstrument.markAsDefaultCreated();
   if (!m_rInstruments.hasSameInstrument(melodicInstrument))
   {
      m_rInstruments.insertMelodicInstrument(melodicInstrument);
   }
}

void InstrumentsMDChangeHandler::add(musicDevice::MusicDevice* pMusicDevice)
{
   assert(pMusicDevice);
   if (pMusicDevice->soundHandler)
   {
      m_rInstruments.fillReferencesToMD(pMusicDevice);
      addDefaultInstrumentsFor(pMusicDevice);
   }
}

void InstrumentsMDChangeHandler::remove(musicDevice::MusicDevice* pMusicDevice)
{
   assert(pMusicDevice);
   if (pMusicDevice->soundHandler)
   {
      m_rInstruments.removeReferencesToMD(pMusicDevice);
   }
}
