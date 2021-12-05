#ifndef UTIL_SETTINGS_H
#define UTIL_SETTINGS_H

#include <JsonCast.h>
#include <Meta.h>
#include <fmt/format.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <string_view>
#include "HomeDir.h"

namespace util
{
constexpr std::string_view localSettingsDir = ".config";

class Settings
{
public:
   inline Settings(const std::string& relDirName,
                   const std::string& fileName) noexcept :
       m_settingsDir(
           fmt::format("{}/{}", util::getLocalSettingsDir(), relDirName)),
       m_fileName(fileName)
   {
   }

   template <typename T> void save(const std::string& section, T&& data) const
   {
      std::filesystem::create_directories(m_settingsDir);
      const std::string filePath =
          fmt::format("{}/{}", m_settingsDir, m_fileName);
      nlohmann::json j;
      {
         std::ifstream settingsFile(filePath);
         if (settingsFile.is_open())
         {
            settingsFile >> j;
         }
      }
      // use tmpfilename to create settings to avoid corruption
      const std::string fileNameTmp = fmt::format("{}_tmp_", filePath);
      std::ofstream settingsFile;
      settingsFile.exceptions(std::ifstream::failbit);
      settingsFile.open(fileNameTmp);
      j[section] = nlohmann::json(std::forward<T>(data));
      settingsFile << std::setw(4) << j;
      std::filesystem::rename(fileNameTmp, filePath);
   }

   template <typename T>
   [[nodiscard]] auto load(const std::string& section) const
   {
      nlohmann::json j;
      std::ifstream settingsFile;
      const std::string filePath =
          fmt::format("{}/{}", m_settingsDir, m_fileName);
      settingsFile.open(filePath);
      if (settingsFile.fail())
      {
         throw std::runtime_error(
             fmt::format("Settings file '{}' not found", filePath));
      }
      settingsFile >> j;
      return j[section].get<T>();
   }

private:
   const std::string m_settingsDir;
   const std::string m_fileName;
};

}   // namespace util

#endif