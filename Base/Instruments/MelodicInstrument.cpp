#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"
#include "SoundSection.h"
#include "VectorIndexInRange.h"

using namespace base::instruments;

MelodicInstrument::MelodicInstrument(std::string name,
                                     std::shared_ptr<RtData> rtData) noexcept :
    m_name(std::move(name)),
    m_pRtData(std::move(rtData))
{
   for (auto& e : m_pRtData->noteAllocations) { e = RtData::FREE; }
}

void MelodicInstrument::noteOn(int note, float velocity) const noexcept
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] != RtData::FREE)
   {
      return;
   }
   if (!m_voices.empty())
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
   }
   m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
   std::for_each(m_voices[m_pRtData->currentVoiceIndex()].voices.begin(),
                 m_voices[m_pRtData->currentVoiceIndex()].voices.end(),
                 [note, velocity](const Voice& voice) {
                    if (voice.pSoundDevice)
                    {
                       voice.pSoundDevice->noteOn(
                           voice.voiceIndex, note + voice.noteOffset, velocity);
                    }
                 });
}

void MelodicInstrument::noteOff(int note, float velocity) const noexcept
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   auto& compositeVoice = m_voices[m_pRtData->noteAllocations[note]];
   std::for_each(compositeVoice.voices.begin(), compositeVoice.voices.end(),
                 [note, velocity](const Voice& voice) {
                    if (voice.pSoundDevice)
                    {
                       voice.pSoundDevice->noteOff(
                           voice.voiceIndex, note + voice.noteOffset, velocity);
                    }
                 });
   m_pRtData->noteAllocations[note] = RtData::FREE;
}

void MelodicInstrument::pitchBend(float value) const noexcept
{
   for (auto& compositeVoice : m_voices)
   {
      std::for_each(compositeVoice.voices.begin(), compositeVoice.voices.end(),
                    [value](const Voice& voice) {
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
                                                bool rr) const noexcept
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
                                                bool rr) const noexcept
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
                                          float value) const noexcept
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
                                          int parameterId,
                                          float value) const noexcept
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

const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int compPart, int parameterIdx) const
{
   return nullptr;
   // TODO
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
