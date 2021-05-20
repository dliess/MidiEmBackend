#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <functional>
#include <vector>

#include "Identifiable.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "Meta.h"
#include "Settings.h"

namespace base
{
namespace musicDevice
{
class MusicDeviceContainer;
}

namespace instruments
{

struct KitInstruments : public std::vector<std::shared_ptr<KitInstrument>>
{
   using Super = std::vector<std::shared_ptr<KitInstrument>>;
   KitInstruments() = default;
   KitInstruments(const Super& rhs) : Super(rhs) {}
   Super::iterator findByUUID(const util::Identifiable::UUID& uuid){
      for(auto iter = Super::begin(); iter != Super::end(); ++iter)
         if(iter->get()->id() == uuid) return iter;
      return Super::end();
   }
   Super::const_iterator findByUUID(const util::Identifiable::UUID& uuid) const{
      for(auto iter = Super::begin(); iter != Super::end(); ++iter)
         if(iter->get()->id() == uuid) return iter;
      return Super::end();
   }
};

struct MelodicInstruments : public std::vector<std::shared_ptr<MelodicInstrument>>
{
   using Super = std::vector<std::shared_ptr<MelodicInstrument>>;
   MelodicInstruments() = default;
   MelodicInstruments(const Super& rhs) : Super(rhs) {}
   Super::iterator findByUUID(const util::Identifiable::UUID& uuid){
      for(auto iter = Super::begin(); iter != Super::end(); ++iter)
         if(iter->get()->id() == uuid) return iter;
      return Super::end();
   }
   Super::const_iterator findByUUID(const util::Identifiable::UUID& uuid) const{
      for(auto iter = Super::begin(); iter != Super::end(); ++iter)
         if(iter->get()->id() == uuid) return iter;
      return Super::end();
   }
};

struct Instruments : public utils::Settings<Instruments>
{
   Instruments(
      musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept;
   struct Data
   {
      KitInstruments kitInstruments;
      MelodicInstruments melodicInstruments;
   };
   Data data;
   // ============== Settings ===============
   using Settings = Data;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================

   using Cb = std::function<void(void)>;
   void registerForDataChange(Cb cb) noexcept;
   void triggerChanged() noexcept;

   void createKitInstrument(std::string name) noexcept;
   void removeKitInstrument(
      const util::Identifiable::UUID& instrumentId) noexcept;

   void createMelodicInstrument(std::string name) noexcept;
   void removeMelodicInstrument(
      const util::Identifiable::UUID& instrumentId) noexcept;

   void createKitInstrumentSound(const util::Identifiable::UUID& instrumentId,
                                 std::string soundName) noexcept;
   void removeKitInstrumentSound(
      const util::Identifiable::UUID& kitSoundId) noexcept;

   void addVoiceToKitInstrumentSound(const util::Identifiable::UUID& kitSoundId,
                                     const musicDevice::MusicDeviceId& soundDeviceId,
                                     int voiceIdx) noexcept;
   void removeVoiceFromInstrumentSound(
      const util::Identifiable::UUID& voiceId) noexcept;

   void addVoiceToMelodicInstrument(
      const util::Identifiable::UUID& instrumentId,
      const musicDevice::MusicDeviceId& soundDeviceId, int voiceIdx) noexcept;
   void removeVoiceFromMelodicInstrument(
      const util::Identifiable::UUID& voiceId) noexcept;

private:
   musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
   std::vector<Cb> m_subscribers;
};

} // namespace instruments
} // namespace base

#include "InstrumentsMeta.h"

#endif