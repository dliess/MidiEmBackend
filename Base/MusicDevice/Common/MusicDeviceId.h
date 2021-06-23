#ifndef MUSIC_DEVICE_ID_H
#define MUSIC_DEVICE_ID_H

#include <string>
#include <Meta.h>

namespace base::musicDevice
{

using MusicDeviceName = std::string; // TODO: struct with manufacturer/product

struct MusicDeviceId
{
    MusicDeviceName deviceName;
    std::string portName;
    inline static const std::string ANY_PORT = "";
    MusicDeviceId() noexcept = default;
    MusicDeviceId(std::string deviceName, std::string portName) noexcept:
        deviceName(std::move(deviceName)), portName(std::move(portName)) 
    {}
    MusicDeviceId(const MusicDeviceId&) = default;
    MusicDeviceId& operator=(const MusicDeviceId&) = default;
    MusicDeviceId(MusicDeviceId&&) noexcept = default;
    MusicDeviceId(std::string deviceIdStr) noexcept
    {
        auto n = deviceIdStr.find("@");
        if(n == std::string::npos)
        {
            deviceName = deviceIdStr;
            return;
        }
        deviceName = deviceIdStr.substr(0, n);
        portName = deviceIdStr.substr(n+1);
    }
    bool operator==(const MusicDeviceId& rhs) const noexcept
    {
        return (deviceName == rhs.deviceName) && 
               (portName.empty() || rhs.portName.empty() || portName == rhs.portName); 
    }
    std::string toStr() const noexcept { return deviceName + "@" + portName; }
};

} // namespace base::musicDevice

#include "MusicDeviceIdMeta.h"

#endif