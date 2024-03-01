#include "MelodicInstrument.h"

#include "Identifiable.h"
#include "MusicDeviceHolder.h"
#include "SoundSection.h"
#include "UtilVectorIndexInRange.h"
#include "InstrumentParameterHandler.h"

using namespace base::instruments::rt;

MelodicInstrument::MelodicInstrument(util::Identifiable::UUIDView uuid) noexcept :
    Instrument(uuid)
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

Void MelodicInstrument::pitchBendMPE(int note, float value)
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).map([&,this](int voiceIdx) {
      for (const auto& sdVoiceRef : m_voices[voiceIdx])
      {
         if (sdVoiceRef)
         {
            sdVoiceRef->soundHandler->pitchBend(sdVoiceRef->sdVoiceIdx, value);
         }
      }
   });
}

Void MelodicInstrument::incrementParameterValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode) 
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::indexOutOfRange);
      for (auto& voice : m_voices)
      {
         auto& sdVoiceRef = voice[componentIdx];
         if(!sdVoiceRef.has_value()) continue;
         ParameterHandler(engine->value(), sdVoiceRef.value()).
            incrementParameterValue(parameterId, parameterAttr, increment, incrementMode);
      }
      return Void{};
   });
}

Void MelodicInstrument::incrementParameterValueEventBound(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode)
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::indexOutOfRange);
      for (auto& voice : m_voices)
      {
         auto& sdVoiceRef = voice[componentIdx];
         if(!sdVoiceRef.has_value()) continue;
         ParameterHandler(engine->value(), sdVoiceRef.value()).
            incrementParameterValueDontCache(parameterId, parameterAttr, increment, incrementMode);
      }
      return Void{};
   });
}

Void MelodicInstrument::incrementParameterValueMPE(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float increment,
    musicDevice::sound::IncrementMode incrementMode)
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).map([&,this](int voiceIdx) {
      auto& sdVoiceRef = m_voices[voiceIdx][componentIdx];
      if (sdVoiceRef)
      {
         ParameterHandler(m_engines[componentIdx].value(), sdVoiceRef.value()).
            incrementParameterValueDontCache(parameterId, parameterAttr, increment, incrementMode);
      }
   });
}

Ret<float> MelodicInstrument::getParameterValue(
    int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Ret<float> {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      return engine->value().parameterCache.getParameter(parameterIdx, parameterAttr);
   });
}

Ret<float> MelodicInstrument::getParameterValue(
    int note, int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).and_then([&,this](int voiceIdx) -> Ret<float> {
      return safe_at(m_voices, voiceIdx).and_then([&,this](auto voice) -> Ret<float> {
         return safe_at(*voice, componentIdx).and_then([&,this](auto sdVoiceRef) -> Ret<float> {
            if (*sdVoiceRef)
            { // TODO
               return sdVoiceRef->value().soundHandler->getParameterValue(sdVoiceRef->value().sdVoiceIdx, parameterIdx, parameterAttr).value_or(0.0f);
            }
            return tl::unexpected(Error::elementEmpty);
         });
      });
   });
}

Void MelodicInstrument::setParameterValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value)
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      for (auto& voice : m_voices)
      {
         auto& sdVoiceRef = voice[componentIdx];
         if(!sdVoiceRef.has_value()) continue;
         ParameterHandler(engine->value(), sdVoiceRef.value()).
            setParameterValue(parameterId, parameterAttr, value);
      }
      return Void{};
   });
}

Void MelodicInstrument::setRelativeParameterValue(
    int componentIdx, int parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float relValue)
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      for (auto& voice : m_voices)
      {
         auto& sdVoiceRef = voice[componentIdx];
         if(!sdVoiceRef.has_value()) continue;
         ParameterHandler(engine->value(), sdVoiceRef.value()).
            setParameterValueDontCache(parameterIdx, parameterAttr, relValue);
      }
      return Void{};
   });
}

Void MelodicInstrument::setParameterValueMPE(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value)
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).map([&,this](int voiceIdx) {
      auto& sdVoiceRef = m_voices[voiceIdx][componentIdx];
      if (sdVoiceRef)
      {
         ParameterHandler(m_engines[componentIdx].value(), sdVoiceRef.value()).
            setParameterValueDontCache(parameterId, parameterAttr, value);
      }
   });
}

Ret<float> MelodicInstrument::fromNormalizedValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Ret<float> {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      for (auto& voice : m_voices)
      {
         auto& sdVoiceRef = voice[componentIdx];
         if(!sdVoiceRef.has_value()) continue;
         return sdVoiceRef.value().soundHandler->
            fromNormalizedValue(sdVoiceRef.value().sdVoiceIdx, parameterId, parameterAttr, percentageValue);
      }//TODO
      return tl::unexpected(Error::elementEmpty);
   });
}

Ret<float> MelodicInstrument::fromNormalizedValue(
    int note, int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr,
    float percentageValue) const
{
   return m_noteAllocation.allocateVoice(note, m_voices.size()).and_then([&,this](int voiceIdx) -> Ret<float> {
      return safe_at(m_voices, voiceIdx).and_then([&,this](auto voice) -> Ret<float> {
         return safe_at(*voice, componentIdx).and_then([&,this](auto sdVoiceRef) -> Ret<float> {
            if (*sdVoiceRef)
            { // TODO
               return sdVoiceRef->value().soundHandler->
                  fromNormalizedValue(sdVoiceRef->value().sdVoiceIdx, parameterId, parameterAttr, percentageValue);
            }
            return tl::unexpected(Error::elementEmpty);
         });
      });
   });
}

Void MelodicInstrument::clearModifier(
    int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr)
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      return engine->value().parameterCache.clearModifier(parameterIdx, parameterAttr);
   });
}

Void MelodicInstrument::applyModifier(
    int componentIdx, std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr, float destination,
    float intensity)
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Void {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      return engine->value().parameterCache.applyModifier(parameterIdx, parameterAttr, destination, intensity);
   });
}

Ret<const base::musicDevice::description::sound::Parameter*>
MelodicInstrument::parameterDescription(int componentIdx,
                                        int parameterIdx) const
{
   return safe_at(m_engines, componentIdx).and_then([&,this](auto engine) -> Ret<const base::musicDevice::description::sound::Parameter*> {
      if(!engine->has_value()) return tl::unexpected(Error::elementEmpty);
      for (const auto& voice : m_voices)
      {
         const auto& sdVoiceRef = voice[componentIdx];
         if (sdVoiceRef)
         {
            return &sdVoiceRef->soundHandler->parameterDescription(sdVoiceRef->sdVoiceIdx, parameterIdx);
         }
      }
      return tl::unexpected(Error::elementEmpty);
   });
}

void MelodicInstrument::updateParameterUI() 
{
   for (auto& engine : m_engines)
   {
      if (engine)
      {
         engine->parameterCache.updateParameterUI();
      }
   }
}

