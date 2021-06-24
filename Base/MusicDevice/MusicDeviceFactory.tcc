#include <loguru.hpp>

#include "MusicDeviceFactory.h"
#include "MusicDevice.h"

namespace base::musicDevice
{
template<typename MidiType, typename MidiMediumType>
std::shared_ptr<MidiType> Factory::createMidi(
   rtmidiadapt::PortIndex index) noexcept
{
   auto pMedium = std::make_unique<MidiMediumType>();
   //LOG_SCOPE_FUNCTION(INFO);
   //VLOG_SCOPE_F(1, "open port");
   if (!pMedium->openPort(index))
   {
      LOG_F(ERROR, "failed to open port with index {}", index);
      return nullptr;
   }
   return std::make_shared<MidiType>(std::move(pMedium));
}

} // namespace base::musicDevice