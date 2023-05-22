#include "KitInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

KitInstrument::KitInstrument(std::string name) noexcept :
    m_name(std::move(name)), m_compositeSounds(16)
{
}

void KitInstrument::noteOn(int note, float velocity, void* token) const
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOn(*si, 64, velocity, token);
   }
}

void KitInstrument::noteOff(int note, float velocity, void* token) const
{
   auto si = toSoundIndex(note);
   if (si)
   {
      noteOff(*si, 64, velocity, token);
   }
}

void KitInstrument::noteOn(int soundIndex, int note, float velocity,
                           void* token) const
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      voice.noteOn(note, velocity);
   }
   rtData->emitNoteOnPlayed(soundIndex + 64, velocity, token);
}

void KitInstrument::noteOff(int soundIndex, int note, float velocity,
                            void* token) const
{
   for (auto& voice : m_compositeSounds[soundIndex].voices)
   {
      voice.noteOff(note, velocity);
   }
   rtData->emitNoteOffPlayed(soundIndex + 64, velocity, token);
}

void KitInstrument::incrementParameterValue(
    int soundIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    bool roundRobin) const
{
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      voice.incrementParameterValue(parameterIdx, parameterAttr, increment,
                                    roundRobin);
   });
}

float KitInstrument::getParameterValue(
    int soundIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   float ret = 0.0;
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      ret = voice.getParameterValue(parameterIdx, parameterAttr);
   });
   return ret;
}

void KitInstrument::setParameterValue(
    int soundIdx, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      voice.setParameterValue(parameterIdx, parameterAttr, value);
   });
}

float KitInstrument::normalizePercentageValue(
    int soundIdx, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   float ret = 0.0;
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      ret = voice.normalizePercentageValue(parameterId, parameterAttr,
                                           percentageValue);
   });
   return ret;
}

const base::musicDevice::description::sound::Parameter*
KitInstrument::parameterDescription(int soundIdx, int componentIdx,
                                    int parameterIdx) const
{
   const base::musicDevice::description::sound::Parameter* ret{nullptr};
   withVoice(soundIdx, componentIdx, [&](const Voice& voice) {
      ret = voice.parameterDescription(parameterIdx);
   });
   return ret;
}

std::optional<int> KitInstrument::toSoundIndex(int note) const
{
   const int noteAdjusted = note - 64;
   if (0 <= noteAdjusted && noteAdjusted < m_compositeSounds.size())
   {
      return noteAdjusted;
   }
   return std::nullopt;
}