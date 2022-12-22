#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H

//#include <map>
#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "CallbackSignal.h"
namespace base::session {
   class Tracks;
}
namespace base::musicDevice
{
class MusicDeviceContainer;

namespace controller
{
class EventRouter
{
public:
   EventRouter(session::Tracks& rTracks, MusicDeviceContainer& rMusicDeviceContainer);
   void createConnection(const EventIdExt& from, const EventDestination& to) noexcept;
   void removeConnection(const EventIdExt& eventIdExt) noexcept;
private:
   session::Tracks& m_rTracks;
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

   void handlePressReleaseDirect(const EventDestination& eventDestination,
                                 const PressReleaseType& value) noexcept;
   void sendNoteOnOff(int note, const EventDestination& eventDestination,
                      const PressReleaseType& value) noexcept;

   void handleContinousValueDirect(const EventDestination& eventDestination,
                                   const ContinousValueType& value) noexcept;
   void sendMPEContinousValue(int note,
                              const EventDestination& eventDestination,
                              const ContinousValueType& value) noexcept;
   void handleRelativeValueDirect(const EventDestination& eventDestination,
                                   const RelativeValueType& value) noexcept;
   void sendMPERelativeValue(int note,
                              const EventDestination& eventDestination,
                              const RelativeValueType& value) noexcept;
   void handleIncrementDirect(const EventDestination& eventDestination,
                              const IncrementType& increment) noexcept;

   CB_SIGNAL(GotConnected, const EventIdExt&, const EventDestination&);
   CB_SIGNAL(GotErased, const EventIdExt&);

   void printMap() const noexcept;
   void retriggerCallbacks();
};

}   // namespace controller
}   // namespace base::musicDevice
#endif