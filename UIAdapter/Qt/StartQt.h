#ifndef QT_ADAPT_START_QT_H
#define QT_ADAPT_START_QT_H

#include <string>

namespace uiadapter::qt
{
int startQt(const std::string& rtServerRpcAddr,
            const std::string& rtServerSignalAddr,
            const std::string& loaderServerRpcAddr,
            const std::string& loaderServerSignalAddr, int& argc, char**& argv);
}   // namespace uiadapter::qt

#endif