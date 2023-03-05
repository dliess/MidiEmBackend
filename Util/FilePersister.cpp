#include "FilePersister.h"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>

#include "HomeDir.h"

using namespace util;

FilePersister::FilePersister(const std::string& relDirName,
                             const std::string& fileName)
{
   const std::string destDir(
       fmt::format("{}/{}", util::getLocalSettingsDir(), relDirName));
   std::filesystem::create_directories(destDir);
   m_filePath = fmt::format("{}/{}", destDir, fileName);
}

void FilePersister::save(std::string_view data)
{
   const std::string filePathTmp = fmt::format("{}_tmp_", m_filePath);
   std::ofstream outFile;
   outFile.exceptions(std::ifstream::failbit);
   outFile.open(filePathTmp);
   outFile.write(data.data(), data.size());
   std::filesystem::rename(filePathTmp, m_filePath);
}

std::string FilePersister::load()
{
   std::ifstream inFile;
   inFile.open(m_filePath);
   if (inFile.fail())
   {
      throw std::runtime_error(
          fmt::format("Settings file '{}' not found", m_filePath));
   }

   inFile.seekg(0, std::ios::end);
   std::streampos length = inFile.tellg();
   inFile.seekg(0, std::ios::beg);

   std::string buffer(length, '\0');
   inFile.read(buffer.data(), length);
   return buffer;
}
