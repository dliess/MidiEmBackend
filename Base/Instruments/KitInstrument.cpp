#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;


KitInstrument::KitInstrument(std::string name) noexcept :
   m_name(std::move(name))
{
}

void VoiceDescr::updateMusicDevicePtr(
   musicDevice::Holder& rMusicDeviceHolder) noexcept
{
   auto iter = rMusicDeviceHolder.musicDevices.findByDeviceId(soundDeviceId);
   if (rMusicDeviceHolder.musicDevices.end() != iter)
   {
      pSoundDevice = iter->second;
   }
}

void KitInstrument::noteOn(int soundIndex, int note, float velocity) noexcept
{
   for (auto& voice : m_sounds.at(soundIndex).voices)
   {
      if (voice.pSoundDevice)
      {
         assert(voice.pSoundDevice->soundHandler);
         voice.pSoundDevice->soundHandler->noteOn(
            voice.voiceIndex, note + voice.noteOffset, velocity);
      }
   }
}

void KitInstrument::noteOff(int soundIndex, int note, float velocity) noexcept
{
   for (auto& voice : m_sounds.at(soundIndex).voices)
   {
      if (voice.pSoundDevice)
      {
         assert(voice.pSoundDevice->soundHandler);
         voice.pSoundDevice->soundHandler->noteOff(
            voice.voiceIndex, note + voice.noteOffset, velocity);
      }
   }
}