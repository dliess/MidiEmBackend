#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"
#include "SoundSection.h"
#include "VectorIndexInRange.h"

using namespace base::instruments;

MelodicInstrument::MelodicInstrument(std::string name,
                                     std::shared_ptr<RtData> rtData) noexcept :
    m_name(std::move(name)), m_pRtData(std::move(rtData))
{
   for (auto& e : m_pRtData->noteAllocations) { e = RtData::FREE; }
}

void MelodicInstrument::noteOn(int note, float velocity, void* token) const
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
   std::for_each(
       m_voices[m_pRtData->currentVoiceIndex()].voices.begin(),
       m_voices[m_pRtData->currentVoiceIndex()].voices.end(),
       [note, velocity](const Voice& voice) { voice.noteOn(note, velocity); });
   rtData->emitNoteOnPlayed(note, velocity, token);
}

void MelodicInstrument::noteOff(int note, float velocity, void* token) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   auto& compositeVoice = m_voices[m_pRtData->noteAllocations[note]];
   std::for_each(
       compositeVoice.voices.begin(), compositeVoice.voices.end(),
       [note, velocity](const Voice& voice) { voice.noteOff(note, velocity); });
   m_pRtData->noteAllocations[note] = RtData::FREE;
   rtData->emitNoteOffPlayed(note, velocity, token);
}

void MelodicInstrument::pitchBend(float value) const
{
   for (auto& compositeVoice : m_voices)
   {
      for (const Voice& voice : compositeVoice.voices)
      {
         voice.pitchBend(value);
      }
   }
}

void MelodicInstrument::incrementParameterValue(int compPart, int parameterId,
                                                float increment, bool rr) const
{
   for (auto& compositeVoice : m_voices)
   {
      if (util::vector_index_in_range(compPart, compositeVoice.voices))
      {
         auto& voice = compositeVoice.voices[compPart];
         voice.incrementParameterValue(parameterId, increment, rr);
      }
   }
}

void MelodicInstrument::incrementParameterValue(int note, int compPart,
                                                int parameterId,
                                                float increment, bool rr) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   const auto& compositeVoice = m_voices[m_pRtData->noteAllocations[note]];
   if (util::vector_index_in_range(compPart, compositeVoice.voices))
   {
      compositeVoice.voices[compPart].incrementParameterValueDontCache(
          parameterId, increment, rr);
   }
}

float MelodicInstrument::getParameterValue(
    int compPart, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return m_voices.at(0).voices.at(compPart).getParameterValue(parameterIdx,
                                                               parameterAttr);
}

float MelodicInstrument::getParameterValue(
    int note, int compPart, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return m_voices.at(m_pRtData->noteAllocations.at(note))
       .voices.at(compPart)
       .getParameterValue(parameterIdx, parameterAttr);
}

void MelodicInstrument::setParameterValue(int compPart, int parameterId,
                                          float value) const
{
   for (auto& compositeVoice : m_voices)
   {
      if (util::vector_index_in_range(compPart, compositeVoice.voices))
      {
         auto& voice = compositeVoice.voices[compPart];
         voice.setParameterValue(parameterId, value);
      }
   }
}

void MelodicInstrument::setParameterValue(int note, int compPart,
                                          int parameterId, float value) const
{
   m_voices.at(m_pRtData->noteAllocations.at(note))
       .voices.at(compPart)
       .setParameterValue(parameterId, value);
}

float MelodicInstrument::normalizePercentageValue(
    int compPart, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return m_voices.at(0).voices.at(compPart).normalizePercentageValue(
       parameterId, parameterAttr, percentageValue);
}

float MelodicInstrument::normalizePercentageValue(
    int note, int compPart, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return m_voices.at(m_pRtData->noteAllocations.at(note))
       .voices.at(compPart)
       .normalizePercentageValue(parameterId, parameterAttr, percentageValue);
}

const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int compPart, int parameterIdx) const
{
   return m_voices.at(0).voices.at(compPart).parameterDescription(parameterIdx);
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
