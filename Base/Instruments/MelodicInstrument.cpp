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
   std::ranges::for_each(m_voices[m_pRtData->currentVoiceIndex()].components,
                         [note, velocity](const Component& component) {
                            component.noteOn(note, velocity);
                         });
   rtData->emitNoteOnPlayed(note, velocity, token);
}

void MelodicInstrument::noteOff(int note, float velocity, void* token) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   auto& voice = m_voices[m_pRtData->noteAllocations[note]];
   std::ranges::for_each(voice.components,
                         [note, velocity](const Component& component) {
                            component.noteOff(note, velocity);
                         });
   m_pRtData->noteAllocations[note] = RtData::FREE;
   rtData->emitNoteOffPlayed(note, velocity, token);
}

void MelodicInstrument::pitchBend(float value) const
{
   for (auto& voice : m_voices)
   {
      for (const Component& component : voice.components)
      {
         component.pitchBend(value);
      }
   }
}

void MelodicInstrument::incrementParameterValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    bool rr) const
{
   for (auto& voice : m_voices)
   {
      if (util::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         component.incrementParameterValue(parameterId, parameterAttr,
                                           increment, rr);
      }
   }
}

void MelodicInstrument::incrementParameterValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    bool rr) const
{ 
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   const auto& voice = m_voices[m_pRtData->noteAllocations[note]];
   if (util::vector_index_in_range(componentIdx, voice.components))
   {
      voice.components[componentIdx].incrementParameterValueDontCache(
          parameterId, parameterAttr, increment, rr);
   }
}

float MelodicInstrument::getParameterValue(
    int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return m_voices.at(LEAD_VOICE_IDX).components.at(componentIdx).getParameterValueCached(
       parameterIdx, parameterAttr);
}

float MelodicInstrument::getParameterValue(
    int note, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return m_voices.at(m_pRtData->noteAllocations.at(note))
       .components.at(componentIdx)
       .getParameterValue(parameterIdx, parameterAttr);
}

void MelodicInstrument::setParameterValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   for (auto& voice : m_voices)
   {
      if (util::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         component.setParameterValue(parameterId, parameterAttr, value);
      }
   }
}

void MelodicInstrument::setParameterValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   m_voices.at(m_pRtData->noteAllocations.at(note))
       .components.at(componentIdx)
       .setParameterValue(parameterId, parameterAttr, value);
}

float MelodicInstrument::normalizePercentageValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return m_voices.at(0).components.at(componentIdx).normalizePercentageValue(
       parameterId, parameterAttr, percentageValue);
}

float MelodicInstrument::normalizePercentageValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return m_voices.at(m_pRtData->noteAllocations.at(note))
       .components.at(componentIdx)
       .normalizePercentageValue(parameterId, parameterAttr, percentageValue);
}

const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int componentIdx, int parameterIdx) const
{
   return m_voices.at(0).components.at(componentIdx).parameterDescription(
       parameterIdx);
}

std::string MelodicInstrument::name() const noexcept { return m_name; }

void MelodicInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}
