#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"
#include "SoundSection.h"
#include "UtilVectorIndexInRange.h"
#include "InstrumentParameterHandler.h"

using namespace base::instruments::rt;

MelodicInstrument::MelodicInstrument(std::string name) noexcept :
    m_name(std::move(name))
{
}

Void MelodicInstrument::noteOn(int note, float velocity, void* token) 
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).map([&,this](int voiceIdx) {
      for(int i = 0; i < m_voices[voiceIdx].size(); ++i)
      {
         auto& sdVoiceRef = m_voices[voiceIdx][i];
          if (sdVoiceRef)
          {
             if(sdVoiceRef->soundHandler->lastplayerId !=
                static_cast<const void*>(this))
             {
                assert(m_engines[i]);
                ParameterHandler(m_engines[i].value(), sdVoiceRef.value()).refreshParameters();
                sdVoiceRef->soundHandler->lastplayerId =
                    static_cast<const void*>(this);
             }
             sdVoiceRef->soundHandler->noteOn(sdVoiceRef->sdVoiceIdx, note, velocity);
          }
      }
      emitNoteOnPlayed(note, velocity, token);
   });
}

Void MelodicInstrument::noteOff(int note, float velocity, void* token) 
{
   return m_noteAllocation.freeVoice(note).map([&,this](int voiceIdx) {
      for(int i = 0; i < m_voices[voiceIdx].size(); ++i)
      {
         auto& sdVoiceRef = m_voices[voiceIdx][i];
         if (sdVoiceRef)
         {
            sdVoiceRef->soundHandler->noteOff(sdVoiceRef->sdVoiceIdx, note, velocity);
         }
      }
      emitNoteOffPlayed(note, velocity, token);
   });
}

void MelodicInstrument::pitchBend(float value) const
{
   for (auto& voice : m_voices)
   {
      for (const auto& sdVoiceRef : voice)
      {
         if (sdVoiceRef)
         {
            sdVoiceRef->soundHandler->pitchBend(sdVoiceRef->sdVoiceIdx, value);
         }
      }
   }
}

void MelodicInstrument::pitchBendMPE(int note, float value) const
{
   if (!mddescrutil::vector_index_in_range(note, m_pRtData->noteAllocations))
   {
      return;
   }
   if (m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
      m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
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
    musicDevice::sound::IncrementMode incrementMode) const
{
   for (auto& voice : m_voices)
   {
      if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         if (component)
         {
            component->incrementParameterValue(parameterId, parameterAttr,
                                               increment, incrementMode);
         }
      }
   }
}

void MelodicInstrument::incrementParameterValueEventBound(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) const
{
   for (auto& voice : m_voices)
   {
      if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         if (component)
         {
            component->incrementParameterValueDontCache(
                parameterId, parameterAttr, increment, incrementMode);
         }
      }
   }
}

void MelodicInstrument::incrementParameterValueMPE(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) const
{
   if (!mddescrutil::vector_index_in_range(note, m_pRtData->noteAllocations))
   {
      return;
   }
   if (m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
      m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
   }
   const auto& voice = m_voices[m_pRtData->noteAllocations[note]];
   if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
   {
      if (voice.components[componentIdx])
      {
         voice.components[componentIdx]->incrementParameterValueDontCache(
             parameterId, parameterAttr, increment, incrementMode);
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
      return component->getSDParameterValue(parameterIdx, parameterAttr);
   }
   return std::nullopt;
}

std::optional<float> MelodicInstrument::getParameterValue(
    int note, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   if (!mddescrutil::vector_index_in_range(note, m_pRtData->noteAllocations))
   {
      return std::nullopt;
   }
   if (m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
      m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
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
      if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         if (component)
         {
            component->setParameterValue(parameterId, parameterAttr, value);
         }
      }
   }
}

void MelodicInstrument::setRelativeParameterValue(
    int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float relValue) const
{
   const auto actVal =
       getParameterValue(componentIdx, parameterIdx, parameterAttr);
   if (actVal)
   {
      for (auto& voice : m_voices)
      {
         if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
         {
            auto& component = voice.components[componentIdx];
            if (component)
            {
               component->setParameterValueDontCache(
                   parameterIdx, parameterAttr, actVal.value() + relValue);
            }
         }
      }
   }
}

void MelodicInstrument::setParameterValueMPE(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   if (!mddescrutil::vector_index_in_range(note, m_pRtData->noteAllocations))
   {
      return;
   }
   if (m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
      m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
   }

   const auto voiceIdx = m_pRtData->noteAllocations.at(note);
   auto& component     = m_voices.at(voiceIdx).components.at(componentIdx);
   if (component)
   {
      component->setParameterValueDontCache(parameterId, parameterAttr, value);
   }
}

float MelodicInstrument::fromNormalizedValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->fromNormalizedValue(parameterId, parameterAttr,
                                            percentageValue);
   }
   return 0.0;   // TODO: exception?
}

float MelodicInstrument::fromNormalizedValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   if (!mddescrutil::vector_index_in_range(note, m_pRtData->noteAllocations))
   {
      return 0.0f;
   }
   if (m_pRtData->noteAllocations[note] == RtData::FREE)
   {
      m_pRtData->incrementVoiceIndex(m_voices.size());
      m_pRtData->noteAllocations[note] = m_pRtData->currentVoiceIndex();
   }

   auto& component = m_voices.at(m_pRtData->noteAllocations.at(note))
                         .components.at(componentIdx);
   if (component)
   {
      return component->fromNormalizedValue(parameterId, parameterAttr,
                                            percentageValue);
   }
   return 0.0f;   // TODO: exception?
}

void MelodicInstrument::clearModifier(
    int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->clearModifier(parameterIdx, parameterAttr);
   }
}

void MelodicInstrument::applyModifier(
    int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float destination,
    float intensity) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->applyModifier(parameterIdx, parameterAttr, destination,
                                      intensity);
   }
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

void MelodicInstrument::updateParameterUI() const
{
   if (m_voices.size())
   {
      for(size_t componentIdx = 0; componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE; ++componentIdx)
      {
         const auto component = getFirstComponent(componentIdx);
         if (component)
         {
            component->updateParameterUI();
         }
      }
   }
}
