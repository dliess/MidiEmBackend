#include "InstrumentsMDChangeHandler.h"

#include "InstrumentComponentParameterCacheCreator.h"
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
   for (int voiceIdx = 0; voiceIdx < voiceDescr.size(); ++voiceIdx)
   {
      KitVoice voice(voiceDescr[voiceIdx].name);
      voice.components.emplace_back(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          nullptr, pMusicDevice->deviceId(), voiceIdx, 0);
      kitInstrument.addVoice(voiceIdx, voice);
   }
   if (!m_rInstruments.hasKitInstrument(kitInstrument))
   {
      m_rInstruments.insertKitInstrument(kitInstrument);
   }
}

void InstrumentsMDChangeHandler::addDefaultInstrumentsForInstrumentPerVoice(
    musicDevice::MusicDevice* pMusicDevice)
{
   const auto& voiceDescr = pMusicDevice->description()->soundSection->voices;
   for (int voiceIdx = 0; voiceIdx < voiceDescr.size(); ++voiceIdx)
   {
      std::string name = pMusicDevice->description()->productName;
      if (voiceDescr.size() > 1)
      {
         name = name + " - " + std::to_string(voiceIdx + 1);
      }
      MelodicInstrument melodicInstrument(
          name, std::make_shared<MelodicInstrument::RtData>());
      melodicInstrument.markAsDefaultCreated();
      MelodicVoice voice(voiceDescr[voiceIdx].name);
      voice.components[0] = Component(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          nullptr, pMusicDevice->deviceId(), voiceIdx, 0);
      melodicInstrument.voices().push_back(std::move(voice));

      if (!m_rInstruments.hasMelodicInstrument(melodicInstrument))
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
   MelodicInstrument melodicInstrument(
       pMusicDevice->description()->productName,
       std::make_shared<MelodicInstrument::RtData>());
   melodicInstrument.markAsDefaultCreated();
   for (int voiceIdx = 0; voiceIdx < voiceDescr.size(); ++voiceIdx)
   {
      MelodicVoice voice(voiceDescr[voiceIdx].name);
      voice.components[0] = Component(
          pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                     : nullptr,
          nullptr, pMusicDevice->deviceId(), voiceIdx, 0);
      melodicInstrument.voices().push_back(std::move(voice));
   }
   if (!m_rInstruments.hasMelodicInstrument(melodicInstrument))
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