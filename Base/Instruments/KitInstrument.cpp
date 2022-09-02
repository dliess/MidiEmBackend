#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)),
    m_compositeSounds(16)
{
}

void KitInstrument::noteOn(int soundIndex, int note, float velocity) noexcept
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      if (voice.pSoundDevice)
      {
         voice.pSoundDevice->noteOn(voice.voiceIndex, note + voice.noteOffset,
                                    velocity);
      }
   }
}

void KitInstrument::noteOff(int soundIndex, int note, float velocity) noexcept
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      if (voice.pSoundDevice)
      {
         voice.pSoundDevice->noteOff(voice.voiceIndex, note + voice.noteOffset,
                                     velocity);
      }
   }
}