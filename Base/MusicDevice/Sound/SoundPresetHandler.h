#ifndef SOUND_PRESET_HANDLER_H
#define SOUND_PRESET_HANDLER_H

#include "DevicePresets.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace base::musicDevice
{
namespace description::sound
{
struct Section;
}
namespace sound
{
class ParameterStorage;

namespace preset
{

class PresetHandler
{
public:
   PresetHandler(const description::sound::Section& rSoundSection,
                 ParameterStorage& rParameterStorage,
                 std::shared_ptr<DevicePresets> pSoundPresets) noexcept;
   std::vector<std::vector<std::string>> getSoundPresetList() const noexcept;
   std::optional<std::string> getActualSoundPresetName(
       int voiceIdx) const noexcept;
   void resetToActualSoundPreset(int voiceIdx) noexcept;
   void selectSoundPreset(int voiceIdx, const std::string& presetName) noexcept;
   void storeAsSoundPreset(int voiceIdx, const std::string& presetName,
                           Category category, Genre genre) noexcept;
   void stageCurrentState(int voiceIdx) noexcept;
   void resetToStaged(int voiceIdx) noexcept;
   void deletePreset(int voiceIdx,
                     const std::string& newSelectedPreset) noexcept;

   std::shared_ptr<DevicePresets> getSoundPresets() const noexcept
   {
      return m_pDevicePresets;
   }

   using ChangedCb = std::function<void(int voiceIdx, const std::string& presetName)>;
   void registerChangedCb(ChangedCb cb) noexcept;
private:
   const description::sound::Section& m_rSoundSection;
   ParameterStorage& m_rParameterStorage;
   std::shared_ptr<DevicePresets> m_pDevicePresets;
   std::vector<ChangedCb> m_changedCbs;
};

}   // namespace preset
}   // namespace sound
}   // namespace base::musicDevice
#endif