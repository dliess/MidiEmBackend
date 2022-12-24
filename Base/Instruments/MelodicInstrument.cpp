#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"
#include "VectorIndexInRange.h"

using namespace base::instruments;

MelodicInstrument::MelodicInstrument(std::string name) noexcept :
    m_name(std::move(name))
{
   for (auto& e : m_noteAllocations) { e = FREE; }
}

void MelodicInstrument::noteOn(int note, float velocity) noexcept
{
   if (!util::vector_index_in_range(note, m_noteAllocations) ||
       m_noteAllocations[note] != FREE)
   {
      return;
   }
   incrementVoiceIndex();
   m_noteAllocations[note] = m_currentVoiceIndex;
   std::for_each(m_voices[m_currentVoiceIndex].voices.begin(),
                 m_voices[m_currentVoiceIndex].voices.end(),
                 [note, velocity](Voice& voice) {
                    if (voice.pSoundDevice)
                    {
                       voice.pSoundDevice->noteOn(
                           voice.voiceIndex, note + voice.noteOffset, velocity);
                    }
                 });
}

void MelodicInstrument::noteOff(int note, float velocity) noexcept
{
   if (!util::vector_index_in_range(note, m_noteAllocations) ||
       m_noteAllocations[note] == FREE)
   {
      return;
   }
   auto& compositeVoice = m_voices[m_noteAllocations[note]];
   std::for_each(compositeVoice.voices.begin(), compositeVoice.voices.end(),
                 [note, velocity](Voice& voice) {
                    if (voice.pSoundDevice)
                    {
                       voice.pSoundDevice->noteOff(
                           voice.voiceIndex, note + voice.noteOffset, velocity);
                    }
                 });
   m_noteAllocations[note] = FREE;
}

void MelodicInstrument::pitchBend(float value) noexcept
{
   for (auto& compositeVoice : m_voices)
   {
      std::for_each(compositeVoice.voices.begin(), compositeVoice.voices.end(),
                    [value](Voice& voice) {
                       if (voice.pSoundDevice)
                       {
                          voice.pSoundDevice->pitchBend(voice.voiceIndex,
                                                        value);
                       }
                    });
   }
}

void MelodicInstrument::incrementParameterValue(int compPart, int parameterId,
                                                float increment,
                                                bool rr) noexcept
{
   // TODO: this is faulty
   for (auto& compositeVoice : m_voices)
   {
      auto& voice = compositeVoice.voices[compPart];
      if (voice.pSoundDevice)
      {
         voice.pSoundDevice->incrementParameterValue(
             voice.voiceIndex, parameterId, increment, rr);
      }
   }
}

void MelodicInstrument::incrementParameterValue(int note, int compPart,
                                                int parameterId,
                                                float increment,
                                                bool rr) noexcept
{
   // TODO: MPR
}

float MelodicInstrument::getParameterValue(
    int compPart, int parameterIdx,
    musicDevice::sound::ParameterPart parameterPart) const noexcept
{
   return 0;
   // TODO
}

float MelodicInstrument::getParameterValue(
    int note, int compPart, int parameterIdx,
    musicDevice::sound::ParameterPart parameterPart) const noexcept
{
   return 0;
   // TODO MPE
}

void MelodicInstrument::setParameterValue(int compPart, int parameterId,
                                          float value) noexcept
{
   // TODO: this is faulty
   for (auto& compositeVoice : m_voices)
   {
      auto& voice = compositeVoice.voices[compPart];
      if (voice.pSoundDevice)
      {
         voice.pSoundDevice->setParameterValue(voice.voiceIndex, parameterId,
                                               value);
      }
   }
}

void MelodicInstrument::setParameterValue(int note, int compPart,
                                          int parameterId, float value) noexcept
{
   // TODO: this is the MPE version
}

float MelodicInstrument::normalizePercentageValue(
    int compPart, int parameterId,
    musicDevice::sound::ParameterPart parameterPart,
    float percentageValue) const noexcept
{
   // TODO
   return 0;
}

float MelodicInstrument::normalizePercentageValue(
    int note, int compPart, int parameterId,
    musicDevice::sound::ParameterPart parameterPart,
    float percentageValue) const noexcept
{
   // TODO MPE
   return 0;
}

MelodicInstrument::VoiceContainer& MelodicInstrument::voices() noexcept
{
   return m_voices;
}

std::string MelodicInstrument::name() const noexcept { return m_name; }

void MelodicInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}
