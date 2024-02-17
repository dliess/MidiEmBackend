#include "KitInstrumentModifier.h"

#include <spdlog/spdlog.h>

using namespace base::instruments::loader;

KitInstrumentModifier::KitInstrumentModifier(KitInstrument& rKitInstrument) noexcept :
    m_rKitInstrument(rKitInstrument)
{
}

void KitInstrumentModifier::renameKitInstrument(std::string name) noexcept
{
   m_rKitInstrument.setName(std::move(name));
   m_rKitInstrument.unmarkAsDefaultCreated();
}

Void KitInstrumentModifier::createNewVoiceInKitInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).map(
      [&,this](auto md) -> void {
         KitVoice voice(md->description()->soundSection->voices[sdVoiceIdx].name);
         voice.components.emplace_back(&md->soundHandler.value(),
                                       md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
                                       md->deviceId(),
                                       sdVoiceIdx, 0);
         m_rKitInstrument.m_voices.push_back(std::move(voice));
         m_rKitInstrument.unmarkAsDefaultCreated();
      });
}

Void KitInstrumentModifier::addComponentToKitInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rFactoryDataHolder.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         if(!md->soundHandler)
         {
            return tl::unexpected(Error::soundHandlerNotAvailable);
         }
         if (m_rKitInstrument.m_voices.operator[](voiceIdx).components.size() >=
             KitVoice::NUM_MAX_COMPONENTS_PER_VOICE)
         {
            return tl::unexpected(Error::maxComponentsPerVoiceReached);
         }
         m_rKitInstrument.m_voices.operator[](voiceIdx).components.emplace_back(
             &md->soundHandler.value(),
             md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
             md->deviceId(),
             sdVoiceIdx, 0);
         m_rKitInstrument.unmarkAsDefaultCreated();
         return Void{};
      });
}

Void KitInstrumentModifier::removeComponentFromKitInstrumentVoice(int voiceIdx,
    int componentIdx) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         voice->components.erase(voice->components.begin() + componentIdx);
         m_rKitInstrument.unmarkAsDefaultCreated();
      });
}

Void KitInstrumentModifier::removeVoiceFromKitInstrument(int voiceIdx) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         m_rKitInstrument.m_voices.erase(m_rKitInstrument.m_voices.begin() + voiceIdx);
         m_rKitInstrument.unmarkAsDefaultCreated();
      });
}

Void KitInstrumentModifier::setNoteOffsetInKitInstrumentComponent(int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         voice->components[componentIdx].setNoteOffset(noteOffset);
         m_rKitInstrument.unmarkAsDefaultCreated();
      });
}

Void KitInstrumentModifier::setNoteOffsetInKitInstrumentVoice(
                                       int voiceIdx, int noteOffset) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         if(voice->noteOffset != noteOffset)
         {
            voice->noteOffset = noteOffset;
            m_rKitInstrument.unmarkAsDefaultCreated();
         }
      });
}

Void KitInstrumentModifier::setVoiceNameInKitInstrument(int voiceIdx,
    const std::string& name) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         if(voice->name != name)
         {
            voice->name = name;
            m_rKitInstrument.unmarkAsDefaultCreated();
         }
      });
}

Void KitInstrumentModifier::setKitComponentAmp(int voiceIdx,
                        int componentIdx, float amp)
{
   m_rKitInstrument.setComponentAmp(voiceIdx, componentIdx, amp);
   return Void{}; // TODO
}

Void KitInstrumentModifier::setKitVoiceAmp(int voiceIdx, float amp)
{
   m_rKitInstrument.setVoiceAmp(voiceIdx, amp);
   return Void{}; // TODO
}

// void KitInstrumentModifier::incKitInstrumentRefCount()
// {
//    instrumentIt->incRefCount();
// }
// void KitInstrumentModifier::decKitInstrumentRefCount()
// {
//    instrumentIt->decRefCount();
// }

Void KitInstrumentModifier::addVoice(int padIdx, KitVoice voice) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, padIdx).map(
      [this, padIdx, srcVoice = std::move(voice) ](auto dstVoice) {
         *dstVoice = std::move(srcVoice);
      });
}

void KitInstrumentModifier::forEachComponent(
    util::function_ref<void(KitComponent&)> f)
{
   for (auto& voice : m_rKitInstrument.m_voices)
   {
      for (auto& component : voice.components)
      {
         f(component);
      }
   }
}

void KitInstrumentModifier::fillReferences(musicDevice::MusicDevice* pMusicDevice)
{
   forEachComponent([&pMusicDevice](KitComponent& component) {
      if (component.m_soundDeviceId == pMusicDevice->deviceId())
      {
         component.setSoundDevicePtr(
             pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
                                        : nullptr);
      }
   });
}
