#include "Instruments.h"

#include <loguru.hpp>

#include "MusicDeviceContainer.h"
#include "MusicDeviceHolder.h"

using namespace base::instruments;

Instruments::Instruments(
   musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
   m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

Instruments::Settings Instruments::getSettings() const noexcept { return data; }

void Instruments::setSettings(const Settings& settings) noexcept
{
   data = settings;
}

void Instruments::registerForDataChange(Cb cb) noexcept
{
   m_subscribers.push_back(cb);
}

void Instruments::triggerChanged() noexcept
{
   for (auto& cb : m_subscribers) cb();
}

void Instruments::createKitInstrument(std::string name) noexcept
{
   data.kitInstruments.push_back(
      std::make_shared<KitInstrument>(std::move(name)));
   triggerChanged();
}

void Instruments::removeKitInstrument(
   const util::Identifiable::UUID& instrumentId) noexcept
{
   for (auto it = data.kitInstruments.begin(); it != data.kitInstruments.end();
        ++it)
   {
      if ((*it)->id() == instrumentId)
      {
         data.kitInstruments.erase(it);
         triggerChanged();
      }
   }
}

void Instruments::createMelodicInstrument(std::string name) noexcept
{
   data.melodicInstruments.push_back(
      std::move(std::make_shared<MelodicInstrument>(std::move(name))));
   triggerChanged();
}

void Instruments::removeMelodicInstrument(
   const util::Identifiable::UUID& instrumentId) noexcept
{
   for (auto it = data.melodicInstruments.begin();
        it != data.melodicInstruments.end(); ++it)
   {
      if ((*it)->id() == instrumentId)
      {
         data.melodicInstruments.erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::createKitInstrumentSound(
   const util::Identifiable::UUID& instrumentId, std::string soundName) noexcept
{
   auto it = std::find_if(
      data.kitInstruments.begin(), data.kitInstruments.end(),
      [&instrumentId](const std::shared_ptr<KitInstrument>& instr) {
         return instr->id() == instrumentId;
      });
   if (it != data.kitInstruments.end())
   {
      (*it)->addSound(KitSound(std::move(soundName)));
      triggerChanged();
      return;
   }
}

void Instruments::removeKitInstrumentSound(
   const util::Identifiable::UUID& kitSoundId) noexcept
{
   for (auto& pKitInstrument : data.kitInstruments)
   {
      auto it = std::find_if(pKitInstrument->sounds().begin(),
                             pKitInstrument->sounds().end(),
                             [&kitSoundId](const KitSound& kitSound) {
                                return kitSound.id() == kitSoundId;
                             });
      if (it != pKitInstrument->sounds().end())
      {
         pKitInstrument->sounds().erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::addVoiceToKitInstrumentSound(
   const util::Identifiable::UUID& kitSoundId,
   const musicDevice::MusicDeviceId& soundDeviceId, int voiceIdx) noexcept
{
   for (auto& pKitInstrument : data.kitInstruments)
   {
      auto it = std::find_if(pKitInstrument->sounds().begin(),
                             pKitInstrument->sounds().end(),
                             [&kitSoundId](const KitSound& kitSound) {
                                return kitSound.id() == kitSoundId;
                             });
      if (it != pKitInstrument->sounds().end())
      {
         VoiceDescr voiceDescriptor;
         voiceDescriptor.soundDeviceId = soundDeviceId;
         voiceDescriptor.voiceIndex    = voiceIdx;
         auto sndDevIt = m_rMusicDeviceContainer.find(soundDeviceId);
         if (sndDevIt != m_rMusicDeviceContainer.end())
         {
            voiceDescriptor.pSoundDevice = sndDevIt->second;
         }
         it->voices.push_back(voiceDescriptor);
         triggerChanged();
         return;
      }
   }
}

void Instruments::removeVoiceFromInstrumentSound(
   const util::Identifiable::UUID& voiceId) noexcept
{
   for (auto& pKitInstrument : data.kitInstruments)
   {
      for (auto& sound : pKitInstrument->sounds())
      {
         auto it = std::find_if(sound.voices.begin(), sound.voices.end(),
                                [&voiceId](const VoiceDescr& voiceDescr) {
                                   return voiceDescr.id() == voiceId;
                                });
         sound.voices.erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::addVoiceToMelodicInstrument(
   const util::Identifiable::UUID& instrumentId,
   const musicDevice::MusicDeviceId& soundDeviceId, int voiceIdx) noexcept
{
   auto it = std::find_if(
      data.melodicInstruments.begin(), data.melodicInstruments.end(),
      [&instrumentId](const std::shared_ptr<MelodicInstrument>& instr) {
         return instr->id() == instrumentId;
      });
   if (it != data.melodicInstruments.end())
   {
      MelodicInstrumentVoice voiceDescriptor;
      voiceDescriptor.soundDeviceId = soundDeviceId;
      voiceDescriptor.voiceIndex    = voiceIdx;
      auto sndDevIt       = m_rMusicDeviceContainer.find(soundDeviceId);
      if (sndDevIt != m_rMusicDeviceContainer.end())
      {
         voiceDescriptor.pSoundDevice = sndDevIt->second;
      }
      triggerChanged();
   }
}

void Instruments::removeVoiceFromMelodicInstrument(
   const util::Identifiable::UUID& voiceId) noexcept
{
   for(auto& pMelodicInstrument : data.melodicInstruments)
   {
      auto it = std::find_if(
         pMelodicInstrument->voices().begin(), pMelodicInstrument->voices().end(),
         [&voiceId](const MelodicInstrumentVoice& voice) {
            return voice.id() == voiceId;
         });
      pMelodicInstrument->voices().erase(it);
   }
}
