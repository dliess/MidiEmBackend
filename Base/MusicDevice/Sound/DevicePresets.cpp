#include "DevicePresets.h"
#include "VectorIndexInRange.h"

using namespace base::musicDevice::sound::preset;

DevicePresets::DevicePresets(std::string manufacturer,
                           std::string product) noexcept :
   m_manufacturer(std::move(manufacturer)),
   m_product(std::move(product))
{
   utils::Settings<DevicePresets>::load(outDirName(), outFileName(),
                                       SETTING_FILE_SECTION);
}

DevicePresets::~DevicePresets()
{
   utils::Settings<DevicePresets>::save(outDirName(), outFileName(),
                                       SETTING_FILE_SECTION);
}

std::vector<std::vector<std::string>> DevicePresets::getSoundPresetList()
   const noexcept
{
   std::vector<std::vector<std::string>> ret(m_presets.size());
   for (int idx = 0; idx < m_presets.size(); ++idx)
   {
      for (const auto& e : m_presets[idx])
      {
         ret[idx].push_back(e.first);
      }
   }
   return ret;
}

const std::vector<float>& DevicePresets::preset(
   int engineIdx, const std::string& presetName) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   const auto it             = enginePresets.find(presetName);
   assert(it != enginePresets.end());
   return it->second;
}

bool DevicePresets::hasPreset(int engineIdx, const std::string& presetName) const noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   const auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];
   return enginePresets.find(presetName) != enginePresets.end();
}

void DevicePresets::savePreset(
   int engineIdx, 
   const std::string& presetName,
   const std::vector<float>& voiceParams) noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];

   const bool isNew      = enginePresets.find(presetName) == enginePresets.end();
   enginePresets[presetName] = voiceParams;
   if (isNew)
   {
      std::lock_guard<std::mutex> lock(m_changedCbMutex);
      if (m_changeCb)
         m_changeCb();
   }
}

std::string DevicePresets::incrementNameIdx(
   int engineIdx,
   const std::string& presetName) const noexcept
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
         LOG_F(ERROR, "Could not get index of preset file '{}', {}", presetName,
               e.what());
         return incrementNameIdx(engineIdx, baseName + "_");
      }
   }
}

void DevicePresets::deletePreset(int engineIdx, const std::string& presetName) noexcept
{
   assert(util::vector_index_in_range(engine2VectorIdx(engineIdx), m_presets));
   auto& enginePresets = m_presets[engine2VectorIdx(engineIdx)];

   enginePresets.erase(presetName);
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   if (m_changeCb)
      m_changeCb();
}

std::string DevicePresets::outDirName() const noexcept
{
   std::string str = "Presets/Devices/" + m_manufacturer;
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

std::string DevicePresets::outFileName() const noexcept
{
   std::string str = m_product + ".json";
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

void DevicePresets::registerPresetListChangeCb(std::function<void()> cb) noexcept
{
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   m_changeCb = cb;
}

void DevicePresets::clearPresetListChangeCb() noexcept
{
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   m_changeCb = nullptr;
}
