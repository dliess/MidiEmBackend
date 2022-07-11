#ifndef MUSIC_DEVICE_FACTORY_DATA_HOLDER_H
#define MUSIC_DEVICE_FACTORY_DATA_HOLDER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "CallbackSignal.h"
#include "DevicePresets.h"
#include "Identifiable.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"
#include "VectorPlusOne.h"

namespace base::musicDevice::factory
{
struct DataHolder
{
   DataHolder(std::string configDir) noexcept;

   void soundDevicesPresetChanged(const sound::preset::Id& enginePresetId);
   void soundDeviceActualPresetNameChanged(
       const util::Identifiable::UUID& uuid, int voiceIdx,
       const std::string& newPresetName) noexcept;

   std::shared_ptr<description::Description> getDescription(
       const MusicDeviceName& deviceName) noexcept;

   std::shared_ptr<sound::preset::DevicePresets> getDevicePresets(
       const MusicDeviceName& deviceName) noexcept;

   const MusicDeviceId* musicDeviceId(const util::Identifiable::UUID& uuid) const noexcept; 

   void addUuid2MdId(const util::Identifiable::UUID& uuid,
                     const MusicDeviceId& mdId) noexcept;
   void removeEntryForUuid(const util::Identifiable::UUID& uuid) noexcept;

   //using ActualPresetNames = util::VectorPlusOne<std::string>;
   using ActualPresetNames = std::vector<std::string>;
   std::shared_ptr<ActualPresetNames> getActualDevicePresetNames(
       const MusicDeviceId& id) noexcept;

   CB_SIGNAL(DescriptionAdded, const std::string&,
             const description::Description&);
   CB_SIGNAL(PresetUpdated, const sound::preset::Id&, sound::preset::Category,
             sound::preset::Genre);
   CB_SIGNAL(PresetRemoved, const sound::preset::Id&);
   CB_SIGNAL(ActualPresetNameChanged, const util::Identifiable::UUID&, int, const std::string&);

   void reEmitSignals();

private:
   const std::string m_configDir;
   std::unordered_map<MusicDeviceName,
                      std::shared_ptr<description::Description>>
       m_descriptionCache;
   std::unordered_map<MusicDeviceName,
                      std::shared_ptr<sound::preset::DevicePresets>>
       m_presetCache;
   std::unordered_map<util::Identifiable::UUID, MusicDeviceId> m_uuidToDevIdMap;
   std::unordered_map<MusicDeviceId, std::shared_ptr<ActualPresetNames>>
       m_actualPresetNames;
};
}   // namespace base::musicDevice::factory

#endif