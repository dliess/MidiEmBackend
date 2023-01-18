#ifndef MUSIC_DEVICE_ID_H
#define MUSIC_DEVICE_ID_H

#include <string>
#include <Meta.h>

namespace base::musicDevice
{

using MusicDeviceName = std::string; // TODO: struct with manufacturer/product
inline std::pair<std::string, std::string> splitDeviceName(const MusicDeviceName& deviceName)
{
    auto n = deviceName.find("/");
    if(n == std::string::npos)
    {
        return std::make_pair(deviceName,"");
    }
    return std::make_pair(deviceName.substr(0, n), deviceName.substr(n+1));
}

struct MusicDeviceId
{
public:
    inline static const std::string ANY_PORT = "";
    MusicDeviceId() noexcept = default;
    MusicDeviceId(std::string deviceName, std::string portName) noexcept:
        m_deviceName(std::move(deviceName)), m_portName(std::move(portName)) 
    {}
    MusicDeviceId(const MusicDeviceId&) = default;
    MusicDeviceId& operator=(const MusicDeviceId&) = default;
    MusicDeviceId(MusicDeviceId&&) noexcept = default;
    MusicDeviceId(const std::string& deviceIdStr) noexcept
    {
        auto n = deviceIdStr.find("@");
        if(n == std::string::npos)
        {
            m_deviceName = deviceIdStr;
            return;
        }
        m_deviceName = deviceIdStr.substr(0, n);
        m_portName = deviceIdStr.substr(n+1);
    }
    bool operator==(const MusicDeviceId& rhs) const noexcept
    {
        return (m_deviceName == rhs.m_deviceName) && 
               (m_portName.empty() || rhs.m_portName.empty() || m_portName == rhs.m_portName);
    }
    std::string toStr() const noexcept { return m_deviceName + "@" + m_portName; }
    const MusicDeviceName& deviceName() const noexcept { return  m_deviceName; }
    const std::string& portName() const noexcept { return m_portName; }
    friend auto meta::registerMembers<MusicDeviceId>();
private:
    MusicDeviceName m_deviceName;
    std::string m_portName;
};

} // namespace base::musicDevice

#include "MusicDeviceIdMeta.h"

#endif