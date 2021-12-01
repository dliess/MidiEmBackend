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

namespace base::musicDevice::sound::preset
{
/*
   Identifier of a preset:
      manufacturer+product + engineIdx + presetName
*/
struct Id
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
   std::optional<int> slotOnDeviceIndex;
   Category category;
   Genre genre;
   std::vector<ParameterData> parameters;
};

namespace settings
{
struct LFOData
{
   void from(
       const ::base::musicDevice::sound::preset::LFOData& lfoData) noexcept;
   ::base::musicDevice::sound::preset::LFOData to() const noexcept;
   std::optional<float> amplitude;
   std::optional<float> frequency;
   std::optional<lfo::Waveform> waveform;
   std::optional<int> multiplierExp;
};

struct ParameterData
{
   void from(const ::base::musicDevice::sound::preset::ParameterData&
                 parameterData) noexcept;
   ::base::musicDevice::sound::preset::ParameterData to() const noexcept;
   float commanded{0.0};
   std::optional<LFOData> lfoData;
};

struct Preset
{
   void from(const ::base::musicDevice::sound::preset::Preset& preset) noexcept;
   ::base::musicDevice::sound::preset::Preset to() const noexcept;
   Category category;
   Genre genre;
   std::vector<ParameterData> parameters;
};

}   // namespace settings

class DevicePresets : public utils::Settings<DevicePresets>
{
public:
   using Super = utils::Settings<DevicePresets>;
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
   MusicDeviceName getMusicDeviceName() const noexcept;
   //              engines                        preset-name  preset-data
   using Presets = std::vector<std::unordered_map<std::string, Preset>>;
   // ============== Settings ===============
   using Settings =
       std::vector<std::unordered_map<std::string, Preset>>;
   Settings getSettings() const noexcept
   {
      return m_presets;
      /* TODO for later
      Settings settings;
      for (const auto& enginePresets : m_presets)
      {
         settings.push_back(settings::value_type());
         for (const auto& preset : enginePresets)
         {

         }
      }
      */
   };
   void setSettings(const Settings& settings) noexcept
   {
      m_presets = settings;
   };
   // =======================================
   void save() const noexcept
   {
      Super::save("EnginePresets", fmt::format("{}.json", m_musicDeviceName),
                  "EnginePresets");
   }
   void load() noexcept
   {
      Super::load("EnginePresets", fmt::format("{}.json", m_musicDeviceName),
                  "EnginePresets");
   }

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