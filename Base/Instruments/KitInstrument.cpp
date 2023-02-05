#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_compositeSounds(16)
{
}

void KitInstrument::noteOn(int note, float velocity) const noexcept
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOn(*si, 64, velocity);
   }
}

void KitInstrument::noteOff(int note, float velocity) const noexcept
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOff(*si, 64, velocity);
   }
}

void KitInstrument::noteOn(int soundIndex, int note,
                           float velocity) const noexcept
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      voice.noteOn(note, velocity);
   }
}

void KitInstrument::noteOff(int soundIndex, int note,
                            float velocity) const noexcept
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      voice.noteOff(note, velocity);
   }
}

void KitInstrument::incrementParameterValue(int soundIdx, int componentIdx,
                                            int parameterIdx, float increment,
                                            bool roundRobin) const noexcept
{
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      voice.incrementParameterValue(parameterIdx, increment, roundRobin);
   });
}

float KitInstrument::getParameterValue(
    int soundIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterPart parameterPart) const noexcept
{
   float ret = 0.0;
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      ret = voice.getParameterValue(parameterIdx, parameterPart);
   });
   return ret;
}

void KitInstrument::setParameterValue(int soundIdx, int componentIdx,
                                      int parameterIdx,
                                      float value) const noexcept
{
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      voice.setParameterValue(parameterIdx, value);
   });
}

float KitInstrument::normalizePercentageValue(
    int soundIdx, int componentIdx, int parameterId,
    musicDevice::sound::ParameterPart parameterPart,
    float percentageValue) const noexcept
{
   float ret = 0.0;
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      ret = voice.normalizePercentageValue(parameterId, parameterPart,
                                           percentageValue);
   });
   return ret;
}

const base::musicDevice::description::sound::Parameter*
KitInstrument::parameterDescription(int soundIdx, int componentIdx,
                                    int parameterIdx) const
{
   // TODO
   return nullptr;
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