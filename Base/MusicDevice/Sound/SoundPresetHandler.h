#ifndef SOUND_PRESET_HANDLER_H
#define SOUND_PRESET_HANDLER_H

#include "DevicePresets.h"
#include "SoundHandlerTypes.h"
#include "CallbackSignal.h"

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
                 std::shared_ptr<DevicePresets> pSoundPresets,
                 std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept;
   void initMidiInHandler(sound::MidiInMsgHandlerT* pMidiInMsgHandler);
   void initMidiOutHandler(sound::MidiOutMsgHandlerT* pMidiOutMsgHandler);
   std::optional<std::string> getActualSoundPresetName(
       int voiceIdx) const noexcept;
   void resetToInitialActualSoundPresetsIfSet() noexcept;
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

   CB_SIGNAL(PresetChanged, int, const std::string&);
private:
   const description::sound::Section& m_rSoundSection;
   ParameterStorage& m_rParameterStorage;
   std::shared_ptr<DevicePresets> m_pDevicePresets;
   sound::MidiInMsgHandlerT* m_pMidiInMsgHandler{nullptr};
   sound::MidiOutMsgHandlerT* m_pMidiOutMsgHandler{nullptr};
   std::shared_ptr<std::vector<std::string>> m_pInitialActualPresetNames;
};

}   // namespace preset
}   // namespace sound
}   // namespace base::musicDevice
#endif