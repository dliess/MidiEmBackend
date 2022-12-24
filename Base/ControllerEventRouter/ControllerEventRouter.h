#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H

//#include <map>
#include <unordered_map>

#include "CallbackSignal.h"
#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
namespace base::session
{
class Tracks;
}

namespace base::instruments
{
struct Instruments;
}

namespace base::musicDevice
{
class MusicDeviceContainer;

namespace controller
{
class EventRouter
{
public:
   EventRouter(session::Tracks& rTracks, instruments::Instruments& rInstruments,
               MusicDeviceContainer& rMusicDeviceContainer);
   void createConnection(const EventIdExt& from,
                         const EventDestination& to) noexcept;
   void removeConnection(const EventIdExt& eventIdExt) noexcept;

private:
   session::Tracks& m_rTracks;
   instruments::Instruments& m_rInstruments;
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

   void handlePressRelease(const EventDestination& eventDestination,
                           const PressReleaseType& value) noexcept;
   void handleAnyWidgetCoordPressRelease(const WidgetCoord& widgetCoord,
                                      const EventDestination& eventDestination,
                                      const PressReleaseType& value) noexcept;
   void handleAnyNotePressRelease(int note,
                               const EventDestination& eventDestination,
                               const PressReleaseType& value) noexcept;

   void handleContinousValue(const EventDestination& eventDestination,
                             const ContinousValueType& value) noexcept;
   void sendMPEContinousValue(int note,
                              const EventDestination& eventDestination,
                              const ContinousValueType& value) noexcept;
   void sendMPEIncrementValue(int note,
                              const EventDestination& eventDestination,
                              const IncrementType& value) noexcept;

   void handleRelativeValue(const EventDestination& eventDestination,
                            const RelativeValueType& value) noexcept;
   void sendMPERelativeValue(int note, const EventDestination& eventDestination,
                             const RelativeValueType& value) noexcept;
   void handleIncrement(const EventDestination& eventDestination,
                        const IncrementType& increment) noexcept;

   void playNoteOnDrumKit(const EventDestination::DrumKit& drumKit,
                          const EventDestination::Note& note,
                          const PressReleaseType& value) noexcept;
   void setParameterOnDrumKit(const EventDestination::DrumKit& drumKit,
                              const EventDestination::Parameter& parameter,
                              const PressReleaseType& value) noexcept;
   void setParameterOnMelodic(const EventDestination::Melodic& melodixc,
                              const EventDestination::Parameter& parameter,
                              const PressReleaseType& value) noexcept;
   void playNoteOnMusicDevice(const EventDestination::MusicDevice& musicDevice,
                              const EventDestination::Note& note,
                              const PressReleaseType& value) noexcept;
   void setParameterOnMusicDevice(
       const EventDestination::MusicDevice& musicDevice,
       const EventDestination::Parameter& parameter,
       const PressReleaseType& value) noexcept;
   void playLayoutMappedDrumKit(const WidgetCoord& widgetCoord,
                                EventDestination::DrumKit& drumKit,
                                const PressReleaseType& value) noexcept;

   CB_SIGNAL(GotConnected, const EventIdExt&, const EventDestination&);
   CB_SIGNAL(GotErased, const EventIdExt&);

   void printMap() const noexcept;
   void retriggerCallbacks();

   static constexpr int ANY = -1;
};

}   // namespace controller
}   // namespace base::musicDevice
#endif