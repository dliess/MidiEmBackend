#ifndef MUSIC_DEVICE_FACTORY_DATA_HOLDER_H
#define MUSIC_DEVICE_FACTORY_DATA_HOLDER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "CallbackSignal.h"
#include "DevicePresets.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceId.h"

namespace base::musicDevice::factory
{
struct DataHolder
{
   DataHolder(std::string configDir) noexcept;

   void soundDevicesPresetChanged(
       const sound::preset::Id& enginePresetId);

   std::shared_ptr<description::Description> getDescription(
       const MusicDeviceName& deviceName) noexcept;

   std::shared_ptr<sound::preset::DevicePresets> getDevicePresets(
       const MusicDeviceName& deviceName) noexcept;

   std::unordered_map<MusicDeviceName,
                      std::shared_ptr<description::Description>>
       descriptionCache;
   std::unordered_map<MusicDeviceName,
                      std::shared_ptr<sound::preset::DevicePresets>>
       presetCache;
   CB_SIGNAL(PresetUpdated, const sound::preset::Id&,
             sound::preset::Category, sound::preset::Genre);
   CB_SIGNAL(PresetRemoved, const sound::preset::Id&);

private:
   const std::string m_configDir;
};
}   // namespace base::musicDevice::factory

#endif