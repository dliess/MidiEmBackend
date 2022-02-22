#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"

using namespace base::instruments;

MelodicInstrument::MelodicInstrument(std::string name) noexcept :
   m_name(std::move(name))
{
   for(auto& e : m_noteAllocations)
   {
      e = FREE;
   }
}

void MelodicInstrument::noteOn(int note, float velocity) noexcept
{
   if(note < 0 || note >= m_noteAllocations.size()) return;
   if (m_noteAllocations[note] != FREE)
      return;
   incrementVoiceIndex();
   m_noteAllocations[note] = m_currentVoiceIndex;
   if (m_voices[m_currentVoiceIndex].pSoundDevice)
   {
      assert(m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler);
      m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler->noteOn(
         m_voices[m_currentVoiceIndex].voiceIndex, note, velocity);
//    spdlog::info( "sending.. {} {} {} {}", m_currentVoiceIndex, m_voices[m_currentVoiceIndex].voiceIndex, note, velocity);
   }
}

void MelodicInstrument::noteOff(int note, float velocity) noexcept
{
   if(note < 0 || note >= m_noteAllocations.size()) return;
   if (m_noteAllocations[note] == FREE)
      return;
   const auto& voice = m_voices[m_noteAllocations[note]];
   voice.pSoundDevice->soundHandler->noteOff(voice.voiceIndex, note, velocity);
   m_noteAllocations[note] = FREE;
}

void MelodicInstrument::noteOn(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if (m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler->noteOn(
         m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::noteOff(int voiceIdx, int note, float velocity) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if (m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler->noteOff(
         m_voices[voiceIndex].voiceIndex, note, velocity);
}

void MelodicInstrument::pitchBend(float value) noexcept
{
   for (auto& voiceDescr : m_voices)
   {
      if (m_voices[m_currentVoiceIndex].pSoundDevice)
         m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler->pitchBend(
            voiceDescr.voiceIndex, value);
   }
}

void MelodicInstrument::pitchBend(int voiceIdx, float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if (m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex].pSoundDevice->soundHandler->pitchBend(
         m_voices[voiceIndex].voiceIndex, value);
}

void MelodicInstrument::parameterChange(int parameterId, float value) noexcept
{
   for (auto& voiceDescr : m_voices)
   {
      if (m_voices[m_currentVoiceIndex].pSoundDevice)
         m_voices[m_currentVoiceIndex]
            .pSoundDevice->soundHandler->setParameterValue(
               voiceDescr.voiceIndex, parameterId, value);
   }
}

void MelodicInstrument::parameterChange(int voiceIdx, int parameterId,
                                        float value) noexcept
{
   const auto voiceIndex = voiceIdx % m_voices.size();
   if (m_voices[m_currentVoiceIndex].pSoundDevice)
      m_voices[m_currentVoiceIndex]
         .pSoundDevice->soundHandler->setParameterValue(
            m_voices[voiceIndex].voiceIndex, parameterId, value);
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
