#ifndef DEVICE_DESCRIPTION_LOADER_H
#define DEVICE_DESCRIPTION_LOADER_H

#include <string>
#include <memory>

#include "MusicDeviceDescription.h"

namespace base::musicDevice::description
{

std::shared_ptr<Description> loadDescription(const std::string& configDir, const std::string& deviceName);

}   // namespace base::musicDevice::description

#endif // DEVICE_DESCRIPTION_LOADER_H