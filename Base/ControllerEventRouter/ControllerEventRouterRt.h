#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_H

#include <unordered_map>

#include "ControllerEventRouterData.h"
#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "DoubleBuffer.h"
#include "InstrumentsRef.h"
#include "MusicDeviceContainerRef.h"
#include "MusicDeviceDescription.h"

namespace base::eventRouter
{

class EventRouterRt
{
public:
   EventRouterRt(const Data& rMap, instruments::InstrumentsRef rInstruments,
                 musicDevice::MusicDeviceContainerRef rMusicDeviceContainer);

   void operator()(const util::Identifiable::UUID uuid,
                   const musicDevice::controller::Event& event);

private:
   const Data& m_rMap;
   instruments::InstrumentsRef m_rInstruments;
   musicDevice::MusicDeviceContainerRef m_rMusicDeviceContainer;
   void handlePressReleaseType(
       const musicDevice::controller::EventIdExt& event,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void handleContinousValueType(
       const musicDevice::controller::EventIdExt& event,
       const musicDevice::controller::ContinousValueType& value) noexcept;
   void handleIncrementType(
       const musicDevice::controller::EventIdExt& event,
       const musicDevice::controller::IncrementType& value) noexcept;
   void handleRelativeValueType(
       const musicDevice::controller::EventIdExt& event,
       const musicDevice::controller::RelativeValueType& value) noexcept;
   void handleRelativeUnlimitedValueType(
       const musicDevice::controller::EventIdExt& event,
       const musicDevice::controller::RelativeUnlimitedValueType& value) noexcept;

   void handlePressRelease(
       const EventDestination& eventDestination,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void handleAnyWidgetCoordPressRelease(
       const musicDevice::controller::WidgetCoord& widgetCoord,
       const EventDestination& eventDestination,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void handleAnyNotePressRelease(
       int note, const EventDestination& eventDestination,
       const musicDevice::controller::PressReleaseType& value) noexcept;

   void handleContinousValue(
       const EventDestination& eventDestination,
       const musicDevice::controller::ContinousValueType& value) noexcept;
   void sendMPEContinousValue(
       int note, const EventDestination& eventDestination,
       const musicDevice::controller::ContinousValueType& value) noexcept;
   void sendMPEIncrementValue(
       int note, const EventDestination& eventDestination,
       const musicDevice::controller::IncrementType& value) noexcept;

   void handleRelativeValue(
       const EventDestination& eventDestination,
       const musicDevice::controller::RelativeValueType& value) noexcept;
   void sendMPERelativeValue(
       int note, const EventDestination& eventDestination,
       const musicDevice::controller::RelativeValueType& value) noexcept;
   void handleRelativeUnlimitedValue(
       const EventDestination& eventDestination,
       const musicDevice::controller::RelativeUnlimitedValueType& value) noexcept;
   void sendMPERelativeUnlimitedValue(
       int note, const EventDestination& eventDestination,
       const musicDevice::controller::RelativeUnlimitedValueType& value) noexcept;
   void handleIncrement(
       const EventDestination& eventDestination,
       const musicDevice::controller::IncrementType& increment) noexcept;

   void playNoteOnDrumKit(
       const EventDestination::DrumKit& drumKit,
       const EventDestination::Note& note,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void setParameterOnDrumKit(
       const EventDestination::DrumKit& drumKit,
       const EventDestination::Parameter& parameter,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void setParameterOnMelodic(
       const EventDestination::Melodic& melodixc,
       const EventDestination::Parameter& parameter,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void playNoteOnMusicDevice(
       const EventDestination::MusicDevice& musicDevice,
       const EventDestination::Note& note,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void setParameterOnMusicDevice(
       const EventDestination::MusicDevice& musicDevice,
       const EventDestination::Parameter& parameter,
       const musicDevice::controller::PressReleaseType& value) noexcept;
   void playLayoutMappedDrumKit(
       const musicDevice::controller::WidgetCoord& widgetCoord,
       const EventDestination::DrumKit& drumKit,
       const musicDevice::controller::PressReleaseType& value) noexcept;

   static constexpr int ANY = -1;
};

}   // namespace base::eventRouter

#endif