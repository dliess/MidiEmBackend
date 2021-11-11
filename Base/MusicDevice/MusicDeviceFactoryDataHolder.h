#ifndef MUSIC_DEVICE_FACTORY_DATA_HOLDER_H
#define MUSIC_DEVICE_FACTORY_DATA_HOLDER_H

#include <memory>
#include <unordered_map>
#include <functional>
#include "MusicDeviceId.h"
#include "MusicDeviceDescription.h"
#include "DevicePresets.h"

namespace base::musicDevice::factory
{
struct DataHolder
{
   DataHolder(std::string configDir) noexcept;
   /*
  void onSoundDevicesPresetChanged(const MusicDeviceName& musicDeviceName,
                                   int engineIdx,
                                   const std::string& parameterIndex);
   */
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

   using PresetUpdatedCb =
       std::function<void(const MusicDeviceName& musicDeviceName, int engineIdx,
                          const std::string& parameterName,
                          sound::preset::Category, sound::preset::Genre)>;
   using PresetRemovedCb =
       std::function<void(const MusicDeviceName& musicDeviceName, int engineIdx,
                          const std::string& parameterName)>;

   std::vector<PresetUpdatedCb> updatedCbs;
   std::vector<PresetRemovedCb> removedCbs;

private:
   const std::string m_configDir;
};
}   // namespace base::musicDevice::factory

#endif