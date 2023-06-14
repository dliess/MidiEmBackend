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
                         [note, velocity](const auto& component) {
                            if (component)
                            {
                               component->noteOn(note, velocity);
                            }
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
                         [note, velocity](const auto& component) {
                            if (component)
                            {
                               component->noteOff(note, velocity);
                            }
                         });
   m_pRtData->noteAllocations[note] = RtData::FREE;
   rtData->emitNoteOffPlayed(note, velocity, token);
}

void MelodicInstrument::pitchBend(float value) const
{
   for (auto& voice : m_voices)
   {
      for (const auto& component : voice.components)
      {
         if (component)
         {
            component->pitchBend(value);
         }
      }
   }
}

void MelodicInstrument::pitchBend(int note, float value) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }
   const auto& voice = m_voices[m_pRtData->noteAllocations[note]];
   for (const auto& component : voice.components)
   {
      if (component)
      {
         component->pitchBend(value);
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
         if (component)
         {
            component->incrementParameterValue(parameterId, parameterAttr,
                                              increment, rr);
         }
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
      if (voice.components[componentIdx])
      {
         voice.components[componentIdx]->incrementParameterValueDontCache(
             parameterId, parameterAttr, increment, rr);
      }
   }
}

std::optional<float> MelodicInstrument::getParameterValue(
    int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->getParameterValueCached(parameterIdx, parameterAttr);
   }
   return std::nullopt;
}

std::optional<float> MelodicInstrument::getParameterValue(
    int note, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return std::nullopt;
   }

   auto& component = m_voices.at(m_pRtData->noteAllocations.at(note))
                         .components.at(componentIdx);
   if (component)
   {
      return component->getParameterValue(parameterIdx, parameterAttr);
   }
   return std::nullopt;
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
         if (component)
         {
            component->setParameterValue(parameterId, parameterAttr, value);
         }
      }
   }
}

void MelodicInstrument::setParameterValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return;
   }

   const auto voiceIdx = m_pRtData->noteAllocations.at(note);
   auto& component = m_voices.at(voiceIdx).components.at(componentIdx);
   if (component)
   {
      component->setParameterValue(parameterId, parameterAttr, value);
   }
}

float MelodicInstrument::normalizePercentageValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->normalizePercentageValue(parameterId, parameterAttr,
                                                 percentageValue);
   }
   return 0.0;   // TODO: exception?
}

float MelodicInstrument::normalizePercentageValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (!util::vector_index_in_range(note, m_pRtData->noteAllocations) ||
       m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      return 0.0;
   }

   auto& component = m_voices.at(m_pRtData->noteAllocations.at(note))
                         .components.at(componentIdx);
   if (component)
   {
      return component->normalizePercentageValue(parameterId, parameterAttr,
                                                 percentageValue);
   }
   return 0.0;   // TODO: exception?
}

const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int componentIdx,
                                        int parameterIdx) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->parameterDescription(parameterIdx);
   }
   return nullptr;   // TODO: exception?
}

std::string MelodicInstrument::name() const noexcept { return m_name; }

void MelodicInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}