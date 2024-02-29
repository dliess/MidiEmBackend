#include "KitInstrumentModifier.h"

#include <spdlog/spdlog.h>

using namespace base::instruments::rt;

KitInstrumentModifier::KitInstrumentModifier(KitInstrument& rKitInstrument) noexcept :
    m_rKitInstrument(rKitInstrument)
{
}

void KitInstrumentModifier::renameKitInstrument(std::string_view name) noexcept
{
   m_rKitInstrument.setName(name);
}

Void KitInstrumentModifier::createNewVoiceInKitInstrument(
    base::musicDevice::MusicDeviceContainer& rMDContainer,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rMDContainer.getMusicDeviceByUUID(sdUuid).map(
      [&,this](auto md) -> void {
         KitVoice voice(md->description()->soundSection->voices[sdVoiceIdx].name);
         voice.components.emplace_back(&md->soundHandler.value(),
                                       md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
                                       md->deviceId(),
                                       sdVoiceIdx, 0);
         m_rKitInstrument.m_voices.push_back(std::move(voice));
      });
}

Void KitInstrumentModifier::addComponentToKitInstrumentVoice(
    base::musicDevice::MusicDeviceContainer& rMDContainer,
    int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return rMDContainer.getMusicDeviceByUUID(sdUuid).and_then(
      [&,this](auto md) -> Void {
         if(!md->soundHandler)
         {
            return tl::unexpected(Error::soundHandlerNotAvailable);
         }
         if (m_rKitInstrument.m_voices[voiceIdx].components.size() >=
             KitVoice::NUM_MAX_COMPONENTS_PER_VOICE)
         {
            return tl::unexpected(Error::maxComponentsPerVoiceReached);
         }
         m_rKitInstrument.m_voices[voiceIdx].components.emplace_back(
             &md->soundHandler.value(),
             md->description()->soundSection->engineBase(sdVoiceIdx)->parameters.size(),
             md->deviceId(),
             sdVoiceIdx, 0);
         return Void{};
      });
}

Void KitInstrumentModifier::removeComponentFromKitInstrumentVoice(int voiceIdx,
    int componentIdx) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         voice->components.erase(voice->components.begin() + componentIdx);
      });
}

Void KitInstrumentModifier::removeVoiceFromKitInstrument(int voiceIdx) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         m_rKitInstrument.m_voices.erase(m_rKitInstrument.m_voices.begin() + voiceIdx);
      });
}

Void KitInstrumentModifier::setNoteOffsetInKitInstrumentComponent(int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   return safe_at(m_rKitInstrument.m_voices, voiceIdx).map(
      [&,this](auto voice) -> void {
         voice->components[componentIdx].setNoteOffset(noteOffset);
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

// Void KitInstrumentModifier::addVoice(int padIdx, KitVoice voice) noexcept
// {
//    return safe_at(m_rKitInstrument.m_voices, padIdx).map(
//       [this, padIdx, srcVoice = std::move(voice) ](auto dstVoice) {
//          *dstVoice = std::move(srcVoice);
//       });
// }
//
// void KitInstrumentModifier::forEachComponent(
//     util::function_ref<void(KitComponent&)> f)
// {
//    for (auto& voice : m_rKitInstrument.m_voices)
//    {
//       for (auto& component : voice.components)
//       {
//          f(component);
//       }
//    }
// }
//
// void KitInstrumentModifier::fillReferences(musicDevice::MusicDevice* pMusicDevice)
// {
//    forEachComponent([&pMusicDevice](KitComponent& component) {
//       if (component.m_soundDeviceId == pMusicDevice->deviceId())
//       {
//          component.setSoundDevicePtr(
//              pMusicDevice->soundHandler ? &pMusicDevice->soundHandler.value()
//                                         : nullptr);
//       }
//    });
// }
//
// bool KitInstrumentModifier::isUnreferencedAndDefaultCreatedFor(base::musicDevice::MusicDevice* pMusicDevice) const
// {
//    auto hasComponentWith = [this](const musicDevice::MusicDeviceId& deviceId) {
//       return std::ranges::any_of(m_rKitInstrument.m_voices, [deviceId](auto& voice) {
//          return std::ranges::any_of(voice.components, [deviceId](auto& component) {
//             return component.m_soundDeviceId == deviceId;
//          });
//       });
//    };
//    return m_rKitInstrument.refCount() == 0 && 
//           m_rKitInstrument.isDefaultCreated() && 
//           hasComponentWith(pMusicDevice->deviceId());
// }
