#ifndef SOUND_PRESETS_H
#define SOUND_PRESETS_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "EnumReflect.h"
#include "Meta.h"
#include "Settings.h"
#include "SoundSection.h"

namespace base::musicDevice::sound
{
namespace preset
{
DECLARE_ENUM(Genre, uint, None, Classic, DBBreaks, House, Industrial, Jazz,
             RBHHop, RockPop, Techno, Dubstep);
DECLARE_ENUM(Category, uint, None, Arp, Bass, Bell, Classic, Drum, Keyboard,
             Lead, Movement, Pad, Poly, SFX, String, User, Voc);
class SoundPresets : public utils::Settings<SoundPresets>
{
public:
   SoundPresets(std::string manufacturer, std::string product) noexcept;
   ~SoundPresets();
   std::vector<std::vector<std::string>> getSoundPresetList() const noexcept;
   const std::vector<float>& preset(
       int engineIdx, const std::string& presetName) const noexcept;
   bool hasSoundPreset(int engineIdx,
                       const std::string& presetName) const noexcept;
   void saveSoundPreset(int engineIdx, const std::string& presetName,
                        const std::vector<float>& voiceParams) noexcept;
   std::string incrementNameIdx(int engineIdx,
                                const std::string& presetName) const noexcept;
   void deleteSoundPreset(int engineIdx,
                          const std::string& presetName) noexcept;

   void registerPresetListChangeCb(std::function<void()> cb) noexcept;
   void clearPresetListChangeCb() noexcept;

   using Presets =
       std::vector<std::unordered_map<std::string, std::vector<float>>>;
   // ============== Settings ===============
   using Settings = Presets;
   Settings getSettings() const noexcept { return m_presets; };
   void setSettings(const Settings& settings) noexcept
   {
      m_presets = settings;
   };
   // =======================================

private:
   Presets m_presets;
   std::string m_manufacturer;
   std::string m_product;
   std::function<void()> m_changeCb;
   std::mutex m_changedCbMutex;
   std::string outDirName() const noexcept;
   std::string outFileName() const noexcept;
   inline static const std::string SETTING_FILE_SECTION = "sound-preset-data";
   inline static int engine2VectorIdx(int engineIdx) noexcept;
};

inline int SoundPresets::engine2VectorIdx(int engineIdx) noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == engineIdx)
   {
      return 0;
   }
   return engineIdx + 1;
}

}   // namespace preset
}   // namespace base::musicDevice::sound
#endif