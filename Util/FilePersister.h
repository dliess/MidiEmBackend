#ifndef UTIL_FILE_PERSISTER_H
#define UTIL_FILE_PERSISTER_H

#include <string_view>

#include "IDataPersister.h"

namespace util
{
class FilePersister : public IDataPersister
{
public:
   FilePersister(const std::string& relDirName, const std::string& fileName);
   void save(std::string_view data) override;
   [[nodiscard]] std::string load() override;

private:
   std::string m_filePath;
};

}   // namespace util

#endif