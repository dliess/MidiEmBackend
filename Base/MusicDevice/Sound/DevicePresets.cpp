#include "DevicePresets.h"

#include "VectorIndexInRange.h"

using namespace base::musicDevice::sound::preset;

DevicePresets::DevicePresets(MusicDeviceName musicDeviceName,
                             int numEngines) noexcept :
    m_musicDeviceName(std::move(musicDeviceName)),
    m_presets(numEngines + 1),
    m_settings(
        fmt::format("EnginePresets/{}", splitDeviceName(m_musicDeviceName).first),
        fmt::format("{}.json", splitDeviceName(m_musicDeviceName).second))
{
}

std::optional<std::pair<Category, Genre>> DevicePresets::getPresetAttributes(
    int engineIdx, const std::string& presetName) const noexcept
{
   if (!util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets))
      return std::nullopt;
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   auto it                   = enginePresets.find(presetName);
   if (it == enginePresets.end())
      return std::nullopt;
   return std::make_pair(it->second.category, it->second.genre);
}

const Preset& DevicePresets::preset(
    int engineIdx, const std::string& presetName) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   const auto it             = enginePresets.find(presetName);
   assert(it != enginePresets.end());
   return it->second;
}

std::optional<std::string> DevicePresets::getPresetNameByPresetSlot(
    int engineIdx, int slotIndex) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   const auto it =
       std::find_if(enginePresets.begin(), enginePresets.end(),
                    [slotIndex](const std::pair<std::string, Preset>& preset) {
                       spdlog::info( "checking {} <-> {}",
                             *preset.second.slotOnDeviceIndex, slotIndex);
                       return preset.second.slotOnDeviceIndex &&
                              (*preset.second.slotOnDeviceIndex == slotIndex);
                    });
   if (it != enginePresets.end())
   {
      return it->first;
   }
   return std::nullopt;
}

bool DevicePresets::hasPreset(int engineIdx,
                              const std::string& presetName) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   return enginePresets.find(presetName) != enginePresets.end();
}

void DevicePresets::savePreset(int engineIdx, const std::string& presetName,
                               Preset&& preset) noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];

   auto it = enginePresets.find(presetName);
   if (it != enginePresets.end())
   {
      it->second = std::move(preset);
   }
   else
   {
      enginePresets.emplace(std::piecewise_construct,
                            std::forward_as_tuple(presetName),
                            std::forward_as_tuple(std::move(preset)));
   }
}

std::string DevicePresets::incrementNameIdx(
    int engineIdx, const std::string& presetName) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];

   static const std::string DELIM = "~";
   auto n                         = presetName.rfind(DELIM);
   if (n == std::string::npos)
   {
      return presetName + DELIM + "1";
   }
   else
   {
      const std::string baseName = presetName.substr(0, n);
      ;
      try
      {
         int index = std::stoi(presetName.substr(n + 1));
         while (enginePresets.end() !=
                enginePresets.find(baseName + DELIM + std::to_string(++index)))
            ;
         return baseName + DELIM + std::to_string(index);
      }
      catch (std::invalid_argument& e)
      {
         spdlog::error( "Could not get index of preset file '{}', {}", presetName,
               e.what());
         return incrementNameIdx(engineIdx, baseName + "_");
      }
   }
}

void DevicePresets::deletePreset(int engineIdx,
                                 const std::string& presetName) noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];

   enginePresets.erase(presetName);
}

std::string DevicePresets::outDirName() const noexcept
{
   auto [manufacturer, product] = splitDeviceName(m_musicDeviceName);
   std::string str              = "Presets/Devices/" + manufacturer;
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

std::string DevicePresets::outFileName() const noexcept
{
   auto [manufacturer, product] = splitDeviceName(m_musicDeviceName);
   std::string str              = product + ".json";
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

void DevicePresets::save() const noexcept
{
   try
   {
      m_settings.save("EnginePresets", m_presets);
   }
   catch (const std::exception& e)
   {
      spdlog::info( "Could not save preset for {}, e: {}", m_musicDeviceName,
            e.what());
   }
}

void DevicePresets::load() noexcept
{
   try
   {
      m_presets = m_settings.load<Presets>("EnginePresets");
   }
   catch (const std::exception& e)
   {
      spdlog::info( "Could not load preset for {}, e: {}", m_musicDeviceName,
            e.what());
   }
}
