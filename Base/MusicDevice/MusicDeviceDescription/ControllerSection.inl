#ifndef CONTROLLER_SECTION_INL
#define CONTROLLER_SECTION_INL


#include "ControllerSection.h"

namespace base::musicDevice::description::controller
{

inline std::optional<int> getDependentPressReleaseEventIdx(
    const controller::Widget& widget)
{
   for (int i = 0; i < widget.events.size(); ++i)
   {
      const auto pressReleaseEvent = mpark::get_if<controller::EventPressRelease>(&widget.events[i]);
      if (pressReleaseEvent && !pressReleaseEvent->independent.value_or(false))
      {
         return i;
      }
   }
   return std::nullopt;
}

} // namespace base::musicDevice::description::controller


#endif