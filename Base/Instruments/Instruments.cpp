#include "Instruments.h"

#include <spdlog/spdlog.h>

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
   data.kitInstruments.emplace_back(std::move(name));
   triggerChanged();
}

void Instruments::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   for (auto it = data.kitInstruments.begin(); it != data.kitInstruments.end();
        ++it)
   {
      if (it->id() == instrumentId)
      {
         data.kitInstruments.erase(it);
         triggerChanged();
      }
   }
}

void Instruments::createMelodicInstrument(std::string name) noexcept
{
   data.melodicInstruments.emplace_back(std::move(name));
   triggerChanged();
}

void Instruments::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   for (auto it = data.melodicInstruments.begin();
        it != data.melodicInstruments.end(); ++it)
   {
      if (it->id() == instrumentId)
      {
         data.melodicInstruments.erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::createKitInstrumentSound(
    const util::Identifiable::UUID& instrumentId,
    std::string soundName) noexcept
{
   auto it = std::find_if(
       data.kitInstruments.begin(), data.kitInstruments.end(),
       [&instrumentId](const KitInstrument& instr) {
          return instr.id() == instrumentId;
       });
   if (it != data.kitInstruments.end())
   {
      it->addSound(CompositeSound(std::move(soundName)));
      triggerChanged();
      return;
   }
}

void Instruments::removeKitInstrumentSound(
    const util::Identifiable::UUID& kitSoundId) noexcept
{
   for (auto& rKitInstrument : data.kitInstruments)
   {
      auto it = std::find_if(
          rKitInstrument.sounds().begin(), rKitInstrument.sounds().end(),
          [&kitSoundId](const CompositeSound& kompositeSound) {
             return kompositeSound.id() == kitSoundId;
          });
      if (it != rKitInstrument.sounds().end())
      {
         rKitInstrument.sounds().erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::addVoiceToKitInstrumentSound(
    const util::Identifiable::UUID& kitSoundId,
    const musicDevice::MusicDeviceId& soundDeviceId, int voiceIdx) noexcept
{
   for (auto& rKitInstrument : data.kitInstruments)
   {
      auto it = std::find_if(
          rKitInstrument.sounds().begin(), rKitInstrument.sounds().end(),
          [&kitSoundId](const CompositeSound& kompositeSound) {
             return kompositeSound.id() == kitSoundId;
          });
      if (it != rKitInstrument.sounds().end())
      {
         Voice voiceDescriptor;
         voiceDescriptor.soundDeviceId = soundDeviceId;
         voiceDescriptor.voiceIndex    = voiceIdx;
         auto sndDevIt = m_rMusicDeviceContainer.findByDeviceId(soundDeviceId);
         if (sndDevIt != m_rMusicDeviceContainer.end())
         {
            voiceDescriptor.pSoundDevice =
                sndDevIt->second->soundHandler
                    ? &sndDevIt->second->soundHandler.value()
                    : nullptr;
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
   for (auto& rKitInstrument : data.kitInstruments)
   {
      for (auto& sound : rKitInstrument.sounds())
      {
         auto it = std::find_if(sound.voices.begin(), sound.voices.end(),
                                [&voiceId](const Voice& voice) {
                                   return voice.id() == voiceId;
                                });
         sound.voices.erase(it);
         triggerChanged();
         return;
      }
   }
}

void Instruments::createNewSlotInMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& soundDeviceUuid, int voiceIdx) noexcept
{
   /*
   auto instrumentIt = std::find_if(
       data.melodicInstruments.begin(), data.melodicInstruments.end(),
       [&instrumentUuid](const std::shared_ptr<MelodicInstrument>& instr) {
          return instr->id() == instrumentId;
       });
   if (it != data.melodicInstruments.end())
   {
      CompositeSound compositeSound;
      compositeSound.name = "TODO";
      MelodicInstrumentVoice voiceDescriptor;
      voiceDescriptor.soundDeviceId = soundDeviceId;
      voiceDescriptor.voiceIndex    = voiceIdx;
      auto sndDevIt = m_rMusicDeviceContainer.findByDeviceId(soundDeviceId);
      if (sndDevIt != m_rMusicDeviceContainer.end())
      {
         voiceDescriptor.pSoundDevice = sndDevIt->second;
      }
      triggerChanged();
   }
   */
}

void Instruments::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& voiceId) noexcept
{
   /*
   for (auto& pMelodicInstrument : data.melodicInstruments)
   {
      auto it = std::find_if(pMelodicInstrument->voices().begin(),
                             pMelodicInstrument->voices().end(),
                             [&voiceId](const MelodicInstrumentVoice& voice) {
                                return voice.id() == voiceId;
                             });
      pMelodicInstrument->voices().erase(it);
   }
   */
}
