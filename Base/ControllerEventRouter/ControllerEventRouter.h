#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H

//#include <map>
#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"

namespace base::musicDevice
{
class MusicDeviceContainer;

namespace controller
{
class EventRouter
{
public:
   EventRouter(MusicDeviceContainer& rMusicDeviceContainer);

private:
   MusicDeviceContainer& m_rMusicDeviceContainer;
   std::unordered_map<EventExt, EventDestination> m_map;
   void handlePressReleaseType(const EventExt& event,
                               const PressReleaseType& value) noexcept;
   void handleContinousValueType(const EventExt& event,
                               const ContinousValueType& value) noexcept;
   void handleIncrementType(const EventExt& event,
                               const IncrementType& value) noexcept;
   void handleRelativeValueType(const EventExt& event,
                               const RelativeValueType& value) noexcept;
};

}   // namespace controller
}   // namespace base::musicDevice
#endif