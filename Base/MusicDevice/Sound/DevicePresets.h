#ifndef DEVICE_PRESETS_H
#define DEVICE_PRESETS_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "EnumReflect.h"
#include "Meta.h"
#include "Settings.h"
#include "SoundSection.h"
#include "LFO.h"

/*
   Identifier of a preset:
      manufacturer+product + engineIdx + presetName
*/

namespace base::musicDevice::sound
{
namespace preset
{
DECLARE_ENUM(Genre, uint, None, Classic, DBBreaks, House, Industrial, Jazz,
             RBHHop, RockPop, Techno, Dubstep);
DECLARE_ENUM(Category, uint, None, Arp, Bass, Bell, Classic, Drum, Keyboard,
             Lead, Movement, Pad, Poly, SFX, String, User, Voc);

struct LFOData
{
   float amplitude{0.0};
   float frequency{0.0};
   lfo::Waveform waveform{lfo::Waveform::Sine};
   int multiplierExp{0};
};

struct ParameterData 
{
   float commanded{0.0};
   LFOData lfoData;
};

struct Preset
{
   Category category;
   Genre    genre;
   std::vector<ParameterData> parameters;
};

class DevicePresets : public utils::Settings<DevicePresets>
{
public:
   DevicePresets(std::string manufacturer, std::string product) noexcept;
   ~DevicePresets();
   std::vector<std::vector<std::string>> getSoundPresetList() const noexcept;
   const std::vector<float>& preset(
       int engineIdx, const std::string& presetName) const noexcept;
   bool hasPreset(int engineIdx, const std::string& presetName) const noexcept;
   void savePreset(int engineIdx, const std::string& presetName,
                   const std::vector<float>& voiceParams) noexcept;
   std::string incrementNameIdx(int engineIdx,
                                const std::string& presetName) const noexcept;
   void deletePreset(int engineIdx, const std::string& presetName) noexcept;

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

inline int DevicePresets::engine2VectorIdx(int engineIdx) noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == engineIdx)
   {
      return 0;
   }
   return engineIdx + 1;
}

}   // namespace preset
}   // namespace base::musicDevice::sound

#include "DevicePresetsMeta.h"
#endif