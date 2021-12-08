#ifndef MUSIC_DEVICE_FACTORY_DATA_HOLDER_H
#define MUSIC_DEVICE_FACTORY_DATA_HOLDER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "CallbackSignal.h"
#include "DevicePresets.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"
#include "Identifiable.h"
#include "VectorPlusOne.h"

namespace base::musicDevice::factory
{
struct DataHolder
{
   DataHolder(std::string configDir) noexcept;

   void soundDevicesPresetChanged(const sound::preset::Id& enginePresetId);

   std::shared_ptr<description::Description> getDescription(
       const MusicDeviceName& deviceName) noexcept;

   std::shared_ptr<sound::preset::DevicePresets> getDevicePresets(
       const MusicDeviceName& deviceName) noexcept;

   void addUuid2MdId(const util::Identifiable::UUID& uuid,
                     const MusicDeviceId& mdId) noexcept;
   std::optional<MusicDeviceId> getMdId(
       const util::Identifiable::UUID& uuid) const noexcept;

   CB_SIGNAL(DescriptionAdded, const std::string&,
             const description::Description&);
   CB_SIGNAL(PresetUpdated, const sound::preset::Id&, sound::preset::Category,
             sound::preset::Genre);
   CB_SIGNAL(PresetRemoved, const sound::preset::Id&);

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
   std::unordered_map<MusicDeviceId, util::VectorPlusOne<std::string>> m_actualPresets;
};
}   // namespace base::musicDevice::factory

#endif