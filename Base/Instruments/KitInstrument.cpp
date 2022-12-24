#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_compositeSounds(16)
{
}

void KitInstrument::noteOn(int note, float velocity) noexcept
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOn(*si, 64, velocity);
   }
}

void KitInstrument::noteOff(int note, float velocity) noexcept
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOff(*si, 64, velocity);
   }
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

void KitInstrument::incrementParameterValue(int soundIdx, int componentIdx,
                                            int parameterIdx, float increment,
                                            bool roundRobin) noexcept
{
   // TODO
}

float KitInstrument::getParameterValue(
    int soundIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterPart parameterPart) const noexcept
{
   // TODO
   return 0;
}

void KitInstrument::setParameterValue(int soundIdx, int componentIdx,
                                      int parameterIdx, float value) noexcept
{
   // TODO
}

float KitInstrument::normalizePercentageValue(
       int soundIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const noexcept
{
   // TODO
   return 0;
}

std::optional<int> KitInstrument::toSoundIndex(int note) const noexcept
{
   const int noteAdjusted = note - 64;
   if (0 <= noteAdjusted && noteAdjusted < m_compositeSounds.size())
   {
      return noteAdjusted;
   }
   return std::nullopt;
}