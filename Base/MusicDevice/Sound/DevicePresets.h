#ifndef DEVICE_PRESETS_H
#define DEVICE_PRESETS_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "EnumReflect.h"
#include "LFO.h"
#include "Meta.h"
#include "MusicDeviceId.h"
#include "Settings.h"
#include "SoundSection.h"

/*
   Identifier of a preset:
      manufacturer+product + engineIdx + presetName
*/

namespace base::musicDevice::sound::preset
{

struct EnginePresetId
{
   MusicDeviceName musicDeviceName;
   int engineIdx;
   std::string presetName;
};

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
   Genre genre;
   std::vector<ParameterData> parameters;
};

class DevicePresets : public utils::Settings<DevicePresets>
{
public:
   using Super = utils::Settings<DevicePresets>;
   DevicePresets(MusicDeviceName musicDeviceName) noexcept;
   ~DevicePresets();
   template<class Cb>
   void forEachPreset(Cb cb) const;
   std::vector<std::vector<std::string>> getSoundPresetList() const noexcept;
   const Preset& preset(int engineIdx,
                        const std::string& presetName) const noexcept;
   bool hasPreset(int engineIdx, const std::string& presetName) const noexcept;
   std::optional<std::pair<Category, Genre>> getPresetAttributes(
       int engineIdx, const std::string& presetName) const noexcept;
   void savePreset(int engineIdx, const std::string& presetName,
                   Category category, Genre genre, Preset&& preset) noexcept;
   std::string incrementNameIdx(int engineIdx,
                                const std::string& presetName) const noexcept;
   void deletePreset(int engineIdx, const std::string& presetName) noexcept;
   MusicDeviceName getMusicDeviceName() const noexcept;

   using Presets = std::vector<std::unordered_map<std::string, Preset>>;
   // ============== Settings ===============
   using Settings = Presets;
   Settings getSettings() const noexcept { return m_presets; };
   void setSettings(const Settings& settings) noexcept
   {
      m_presets = settings;
   };
   // =======================================
   void save() const noexcept { Super::save("EnginePresets", m_musicDeviceName, "EnginePresets"); }
   void load() noexcept { Super::load("EnginePresets", m_musicDeviceName, "EnginePresets"); }

private:
   Presets m_presets;
   const MusicDeviceName m_musicDeviceName;
   std::mutex m_mutex;
   std::string outDirName() const noexcept;
   std::string outFileName() const noexcept;
   inline static const std::string SETTING_FILE_SECTION = "sound-preset-data";
   inline static int engine2VectorIdx(int engineIdx) noexcept;
};

}   // namespace base::musicDevice::sound::preset

#include "DevicePresets.inl"
#include "DevicePresetsMeta.h"
#endif