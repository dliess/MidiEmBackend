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
         KitInstrument::Voice voice(md->description()->soundSection->voices[sdVoiceIdx].name);
         voice.components.emplace_back(md->description()->soundSection->engineBase(sdVoiceIdx)->parameters,
                                       md->deviceId(),
                                       sdVoiceIdx);
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
             KitInstrument::Voice::NUM_MAX_COMPONENTS_PER_VOICE)
         {
            return tl::unexpected(Error::maxComponentsPerVoiceReached);
         }
         m_rKitInstrument.m_voices.operator[](voiceIdx).components.emplace_back(
             md->description()->soundSection->engineBase(sdVoiceIdx)->parameters,
             md->deviceId(),
             sdVoiceIdx);
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
         voice->components[componentIdx].noteOffset = noteOffset;
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
   return m_rKitInstrument.setComponentAmp(voiceIdx, componentIdx, amp);
}

Void KitInstrumentModifier::setKitVoiceAmp(int voiceIdx, float amp)
{
   return m_rKitInstrument.setVoiceAmp(voiceIdx, amp);
}

Void KitInstrumentModifier::addVoice(int padIdx, KitInstrument::Voice voice) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, padIdx).map(
      [this, padIdx, srcVoice = std::move(voice) ](auto dstVoice) {
         *dstVoice = std::move(srcVoice);
      });
}

void KitInstrumentModifier::forEachComponent(
    util::function_ref<void(KitInstrument::Voice::Component&)> f)
{
   for (auto& voice : m_rKitInstrument.m_voices)
   {
      for (auto& component : voice.components)
      {
         f(component);
      }
   }
}

bool KitInstrumentModifier::isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const
{
   auto hasComponentWith = [this](const musicDevice::MusicDeviceId& deviceId) {
      return std::ranges::any_of(m_rKitInstrument.m_voices, [deviceId](auto& voice) {
         return std::ranges::any_of(voice.components, [deviceId](auto& component) {
            return component.soundDeviceId == deviceId;
         });
      });
   };
   return m_rKitInstrument.refCount() == 0 && 
          m_rKitInstrument.isDefaultCreated() && 
          hasComponentWith(pMusicDevice->deviceId());
}
   
void KitInstrumentModifier::fillDescrReferences(base::musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept
{
   for (auto& voice : m_rKitInstrument.m_voices)
   {
      for (auto& component : voice.components)
      {
         auto mdDescr = rFactoryDataHolder.getDescriptionByMdName(component.soundDeviceId.deviceName());
         if (mdDescr)
         {
            component.paramDescr = &mdDescr.value()->soundSection->engineBase(component.sdVoiceIdx)->parameters;
         }
         else
         {
            spdlog::error("KitInstrumentModifier::fillDescrReferences: MusicDevice description not found for {}", component.soundDeviceId.deviceName());
         }
         if(component.paramDescr->size() != component.parameterData.size())
         {
            spdlog::error("KitInstrumentModifier::fillDescrReferences: Parameter cache size mismatch for {} ... RESIZING", component.soundDeviceId.deviceName());
            component.parameterData.resize(component.paramDescr->size());
         }
      }
   }
}
