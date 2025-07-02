#include <spdlog/spdlog.h>

#include "MusicDeviceFactory.h"
#include "MusicDevice.h"

namespace base::musicDevice::factory
{

inline
const MusicDevices& Factory::musicDevices() const noexcept
{
   return m_musicDevices;
}

inline
MusicDevices& Factory::musicDevices() noexcept
{
   return m_musicDevices;
}

template<typename MidiType, typename MidiMediumType>
std::shared_ptr<MidiType> Factory::createMidi(
   rtmidiadapt::PortIndex index) noexcept
{
   auto pMedium = std::make_unique<MidiMediumType>();
   //LOG_SCOPE_FUNCTION(INFO);
   //VLOG_SCOPE_F(1, "open port");
   if (!pMedium->openPort(index))
   {
      spdlog::error( "failed to open port with index {}", index);
      return nullptr;
   }
   return std::make_shared<MidiType>(std::move(pMedium));
}

template<typename MidiType, typename MidiMediumType>
std::shared_ptr<MidiType> Factory::createVirtualMidi(
   std::string portName) noexcept
{
   auto pMedium = std::make_unique<MidiMediumType>();
   //LOG_SCOPE_FUNCTION(INFO);
   //VLOG_SCOPE_F(1, "open port");
   if (!pMedium->openVirtualPort(std::move(portName)))
   {
      spdlog::error( "failed to open port");
      return nullptr;
   }
   return std::make_shared<MidiType>(std::move(pMedium));
}

} // namespace base::musicDevice::factory
