#ifndef SOUND_PRESET_HANDLER_H
#define SOUND_PRESET_HANDLER_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace base::musicDevice
{
namespace description::sound { struct Section; }
namespace sound
{
class ParameterStorage;

namespace preset
{
class SoundPresets;
class PresetHandler
{
public:
   PresetHandler(const description::sound::Section& rSoundSection,
                 ParameterStorage& rParameterStorage,
                 std::shared_ptr<SoundPresets> pSoundPresets) noexcept;
   std::vector<std::vector<std::string>> getSoundPresetList() const noexcept;
   std::optional<std::string> getActualSoundPresetName(int voiceIdx) const
      noexcept;
   void registerPresetListChangeCb(std::function<void()> cb) noexcept;
   void clearPresetListChangeCb() noexcept;
   void resetToActualSoundPreset(int voiceIdx) noexcept;
   void selectSoundPreset(int voiceIdx, const std::string& presetName) noexcept;
   void storeAsSoundPreset(int voiceIdx,
                           const std::string& presetName) noexcept;
   void resetParametersToZeroState(int voiceIdx) noexcept;
   void deleteSoundPreset(int voiceIdx,
                          const std::string& newSelectedPreset) noexcept;

   std::shared_ptr<SoundPresets> getSoundPresets() const noexcept
   {
      return m_pSoundPresets;
   }

private:
   const base::musicDevice::description::sound::Section& m_rSoundSection;
   ParameterStorage&                                     m_rParameterStorage;
   std::shared_ptr<SoundPresets>                         m_pSoundPresets;
};

} // namespace preset
} // namespace sound
} // namespace base::musicDevice
#endif