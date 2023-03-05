#ifndef UTIL_DATA_PERSISTER_H
#define UTIL_DATA_PERSISTER_H

#include <string_view>
#include <string>

namespace util
{

class IDataPersister
{
public:
    virtual ~IDataPersister() = default;
    virtual void save(std::string_view data) = 0;
    virtual std::string load() = 0;
};

}   // namespace util

#endif