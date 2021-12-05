#ifndef DEVICE_PRESETS_H
#define DEVICE_PRESETS_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "DevicePresetsTypes.h"
#include "Settings.h"
#include "SoundSection.h"

namespace base::musicDevice::sound::preset
{

class DevicePresets
{
public:
   DevicePresets(MusicDeviceName musicDeviceName, int numEngines) noexcept;

   template <class Cb> void forEachPreset(Cb cb) const;
   const Preset& preset(int engineIdx,
                        const std::string& presetName) const noexcept;
   std::optional<std::string> getPresetNameByPresetSlot(int engineIdx, int slotIndex) const noexcept;
   bool hasPreset(int engineIdx, const std::string& presetName) const noexcept;
   std::optional<std::pair<Category, Genre>> getPresetAttributes(
       int engineIdx, const std::string& presetName) const noexcept;
   void savePreset(int engineIdx, const std::string& presetName,
                   Preset&& preset) noexcept;
   std::string incrementNameIdx(int engineIdx,
                                const std::string& presetName) const noexcept;
   void deletePreset(int engineIdx, const std::string& presetName) noexcept;
   using PresetsPerEngine = std::unordered_map<std::string, Preset>;
   using Presets = std::vector<PresetsPerEngine>;
   void save() const noexcept;
   void load() noexcept;

private:
   Presets m_presets;
   const MusicDeviceName m_musicDeviceName;
   std::mutex m_mutex;
   util::Settings m_settings;
   std::string outDirName() const noexcept;
   std::string outFileName() const noexcept;
   inline static const std::string SETTING_FILE_SECTION = "sound-preset-data";
   inline static int engine2VectorIdx(int engineIdx) noexcept;
};

}   // namespace base::musicDevice::sound::preset

#include "DevicePresets.inl"
#include "DevicePresetsTypesMeta.h"
#endif