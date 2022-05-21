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
   std::unordered_map<EventIdExt, EventDestination> m_map;
   void handlePressReleaseType(const EventIdExt& event,
                               const PressReleaseType& value) noexcept;
   void handleContinousValueType(const EventIdExt& event,
                                 const ContinousValueType& value) noexcept;
   void handleIncrementType(const EventIdExt& event,
                            const IncrementType& value) noexcept;
   void handleRelativeValueType(const EventIdExt& event,
                                const RelativeValueType& value) noexcept;

   void handlePressReleaseDirect(const EventIdExt& eventIdExt,
                                 const EventDestination& eventDestination,
                                 const PressReleaseType& value) noexcept;
   void sendNoteOnOff(int note, const EventDestination& eventDestination,
                      const PressReleaseType& value) noexcept;
};

}   // namespace controller
}   // namespace base::musicDevice
#endif