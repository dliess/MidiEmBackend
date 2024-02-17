#include "KitInstrumentsModifier.h"
#include "KitInstrumentModifier.h"
#include <spdlog/spdlog.h>

using namespace base::instruments::loader;

Ret<KitInstruments::iterator> 
KitInstrumentsModifier::getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept
{
   auto instrumentIt = std::ranges::find_if(                                 
       m_rKitInstruments,                                                
       [&instrumentUuid](const KitInstrument& instr) {                   
          return instr.id() == instrumentUuid;                               
       });                                                                   
   if (instrumentIt == m_rKitInstruments.end())                          
   {                                                                         
      return tl::unexpected(Error::uuidNotFound);                                                            
   }
   return instrumentIt;
}

KitInstrumentsModifier::KitInstrumentsModifier(KitInstruments& rKitInstruments) noexcept :
    m_rKitInstruments(rKitInstruments)
{
}

void KitInstrumentsModifier::insertKitInstrument(
    KitInstrument kitInstrument) noexcept
{
   m_rKitInstruments.push_back(std::move(kitInstrument));
}

Void KitInstrumentsModifier::removeKitInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   return getInstrument(instrumentId).map([this](auto instrumentIt) -> void {
      m_rKitInstruments.erase(instrumentIt);
   });
}

Void KitInstrumentsModifier::renameKitInstrument(
    const util::Identifiable::UUID& instrumentId, std::string name) noexcept
{
   return getInstrument(instrumentId).map([this, name = std::move(name)](auto instrumentIt) -> void {
      KitInstrumentModifier(*instrumentIt).renameKitInstrument(std::move(name));
   });
}

Void KitInstrumentsModifier::createNewVoiceInKitInstrument(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).createNewVoiceInKitInstrument(rFactoryDataHolder, sdUuid, sdVoiceIdx);
      });
}

Void KitInstrumentsModifier::addComponentToKitInstrumentVoice(
    base::musicDevice::factory::DataHolder& rFactoryDataHolder,
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).addComponentToKitInstrumentVoice(rFactoryDataHolder, voiceIdx, sdUuid, sdVoiceIdx);
      });
}

Void KitInstrumentsModifier::moveKitInstrumentComponent(
    const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
    int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
    int dstVoiceIdx) noexcept
{
   return getInstrument(srcInstrumentUuid).and_then(
      [&](auto srcInstrumentIt) -> Void {
         return getInstrument(dstInstrumentUuid).and_then(
            [&](auto dstInstrumentIt) -> Void {
               return safe_at(srcInstrumentIt->m_voices, srcVoiceIdx).and_then(
                  [&](auto srcVoice) -> Void {
                     return safe_at(dstInstrumentIt->m_voices, dstVoiceIdx).and_then(
                        [&](auto dstVoice) -> Void {
                           return safe_at(srcVoice->components, srcComponentIdx).and_then(
                              [&](auto srcComponent) -> Void {
                                 dstVoice->components.push_back(*srcComponent);
                                 srcInstrumentIt->unmarkAsDefaultCreated();
                                 dstInstrumentIt->unmarkAsDefaultCreated();
                                 return removeComponentFromKitInstrumentVoice(srcInstrumentUuid, srcVoiceIdx, srcComponentIdx);
                              });
                        });
                  }); 
            });
      });
}

Void KitInstrumentsModifier::removeComponentFromKitInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).removeComponentFromKitInstrumentVoice(voiceIdx, componentIdx);
      });
}

Void KitInstrumentsModifier::removeVoiceFromKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).removeVoiceFromKitInstrument(voiceIdx);
      });
}

Void KitInstrumentsModifier::setNoteOffsetInKitInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx, int noteOffset) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).setNoteOffsetInKitInstrumentComponent(voiceIdx, componentIdx, noteOffset);
      });
}

Void KitInstrumentsModifier::setNoteOffsetInKitInstrumentVoice(const util::Identifiable::UUID& instrumentUuid,
                                       int voiceIdx, int noteOffset) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).setNoteOffsetInKitInstrumentVoice(voiceIdx, noteOffset);
      });
}

Void KitInstrumentsModifier::setVoiceNameInKitInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const std::string& name) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).setVoiceNameInKitInstrument(voiceIdx, name);
      });
}
Void KitInstrumentsModifier::setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                        int componentIdx, float amp)
{
   return getInstrument(uuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).setKitComponentAmp(voiceIdx, componentIdx, amp);
      });
}

Void KitInstrumentsModifier::setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp)
{
   return getInstrument(uuid).and_then(
      [&](auto instrumentIt) -> Void {
         return KitInstrumentModifier(*instrumentIt).setKitVoiceAmp(voiceIdx, amp);
      });
}

void KitInstrumentsModifier::fillReferences(musicDevice::MusicDevice* pMusicDevice)
{
   for (auto& kitInstrument : m_rKitInstruments)
   {
      KitInstrumentModifier(kitInstrument).fillReferences(pMusicDevice);
   }
}
// void KitInstrumentsModifier::incKitInstrumentRefCount(
//     const util::Identifiable::UUID& instrumentUuid)
// {
//    GET_KIT_INSTR_OR_RETURN(instrumentUuid);
//    instrumentIt->incRefCount();
// }
// void KitInstrumentsModifier::decKitInstrumentRefCount(
//     const util::Identifiable::UUID& instrumentUuid)
// {
//    GET_KIT_INSTR_OR_RETURN(instrumentUuid);
//    instrumentIt->decRefCount();
// }
