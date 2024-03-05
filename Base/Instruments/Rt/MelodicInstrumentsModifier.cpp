#include "MelodicInstrumentsModifier.h"
#include "MelodicInstrumentModifier.h"

#include <spdlog/spdlog.h>
#include <memory>
#include "Identifiable.h"


using namespace base::instruments::rt;

Ret<MelodicInstruments::iterator> 
MelodicInstrumentsModifier::getInstrument(util::Identifiable::UUIDView instrumentUuid) noexcept
{
   auto instrumentIt = std::ranges::find_if(                                 
       m_rMelodicInstruments,                                                
       [&instrumentUuid](const MelodicInstrument& instr) {                   
          return instr.id() == instrumentUuid;                               
       });                                                                   
   if (instrumentIt == m_rMelodicInstruments.end())                          
   {                                                                         
      return tl::unexpected(Error::uuidNotFound);
   }
   return instrumentIt;
}

MelodicInstrumentsModifier::MelodicInstrumentsModifier(MelodicInstruments& rMelodicInstruments) noexcept :
    m_rMelodicInstruments(rMelodicInstruments)
{
}

void MelodicInstrumentsModifier::createMelodicInstrument(util::Identifiable::UUIDView uuid) noexcept
{
   m_rMelodicInstruments.push_back(MelodicInstrument(uuid));
}

// void MelodicInstrumentsModifier::insertMelodicInstrument(
//     MelodicInstrument melodicInstrument) noexcept
// {
//    m_rMelodicInstruments.push_back(std::move(melodicInstrument));
// }
//
Void MelodicInstrumentsModifier::removeMelodicInstrument(
    const util::Identifiable::UUID& instrumentId) noexcept
{
   return getInstrument(instrumentId).map([this](auto instrumentIt) -> void {
      m_rMelodicInstruments.erase(instrumentIt);
   });
}

Void MelodicInstrumentsModifier::createNewVoiceInMelodicInstrument(
    base::musicDevice::MusicDeviceContainer& rMDContainer,
    int componentIdx,
    const util::Identifiable::UUID& instrumentUuid,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
        return MelodicInstrumentModifier(*instrumentIt).createNewVoiceInMelodicInstrument(rMDContainer, componentIdx, sdUuid, sdVoiceIdx);
   });
}

Void MelodicInstrumentsModifier::addComponentToMelodicInstrumentVoice(
    base::musicDevice::MusicDeviceContainer& rMDContainer,
    int componentIdx,
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    const util::Identifiable::UUID& sdUuid, int sdVoiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&,this](auto instrumentIt) -> Void {
         return MelodicInstrumentModifier(*instrumentIt).addComponentToMelodicInstrumentVoice(rMDContainer, componentIdx, voiceIdx, sdUuid, sdVoiceIdx);
   });
}

Void MelodicInstrumentsModifier::removeComponentFromMelodicInstrumentVoice(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
    int componentIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return MelodicInstrumentModifier(*instrumentIt).removeComponentFromMelodicInstrumentVoice(voiceIdx, componentIdx);
   });
}

Void MelodicInstrumentsModifier::removeVoiceFromMelodicInstrument(
    const util::Identifiable::UUID& instrumentUuid, int voiceIdx) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return MelodicInstrumentModifier(*instrumentIt).removeVoiceFromMelodicInstrument(voiceIdx);
      });
}

Void MelodicInstrumentsModifier::setNoteOffsetInMelodicInstrumentComponent(
    const util::Identifiable::UUID& instrumentUuid, 
    int componentIdx, int noteOffset) noexcept
{
   return getInstrument(instrumentUuid).and_then(
      [&](auto instrumentIt) -> Void {
         return MelodicInstrumentModifier(*instrumentIt).setNoteOffsetInMelodicInstrumentComponent(componentIdx, noteOffset);
      });
}

Void MelodicInstrumentsModifier::setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                            int componentIdx, float amp)
{
   return getInstrument(uuid).and_then(
      [&](auto instrumentIt) -> Void {
         return MelodicInstrumentModifier(*instrumentIt).setMelodicComponentAmp(componentIdx, amp);
      });
}

// void MelodicInstrumentsModifier::fillReferences(
//     musicDevice::MusicDevice* pMusicDevice)
// {
//    for(auto& instrument : m_rMelodicInstruments)
//    {
//       MelodicInstrumentModifier(instrument).fillReferences(pMusicDevice);
//    }
// }
//
// void MelodicInstrumentsModifier::removeReferences(musicDevice::MusicDevice* pMusicDevice)
// {
//    auto it = m_rMelodicInstruments.begin();
//    while (it != m_rMelodicInstruments.end())
//    {
//       if (MelodicInstrumentModifier(*it).isUnreferencedAndDefaultCreatedFor(pMusicDevice))
//       {
//          it = m_rMelodicInstruments.erase(it);
//       }
//       else
//       {
//          ++it;
//       }
//    }
// }
//
// Void MelodicInstrumentsModifier::incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
// {
//    return getInstrument(uuid).map([](auto instrumentIt) -> void {
//       instrumentIt->incRefCount();
//    });
// }
//
// Void MelodicInstrumentsModifier::decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
// {
//    return getInstrument(uuid).map([](auto instrumentIt) -> void {
//       instrumentIt->decRefCount();
//    });
// }
