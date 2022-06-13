#include <spdlog/spdlog.h>

#include "MusicDeviceFactory.h"
#include "MusicDevice.h"

namespace base::musicDevice::factory
{

inline
const DataHolder& Factory::dataHolder() const noexcept
{
   return m_dataHolder;
}

inline
DataHolder& Factory::dataHolder() noexcept
{
   return m_dataHolder;
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