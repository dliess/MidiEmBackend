#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_H

//#include <map>
#include <unordered_map>

#include "CallbackSignal.h"
#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "InstrumentsRef.h"
#include "MusicDeviceContainerRef.h"
#include "MusicDeviceDescription.h"
#include "DoubleBuffer.h"


namespace base::eventRouter
{
class EventRouter
{
public:
   EventRouter(instruments::InstrumentsRef rInstruments,
               musicDevice::MusicDeviceContainerRef rMusicDeviceContainer);
   void createConnection(const musicDevice::controller::EventIdExt& from,
                         const EventDestination& to) noexcept;
   void removeConnection(
       const musicDevice::controller::EventIdExt& eventIdExt) noexcept;
   void removeConnectionToDestination(const EventDestination& to) noexcept;

   void onControllerDevEventOccured(
       const util::Identifiable::UUID& uuid,
       const musicDevice::controller::Event& event);

   // void musicDeviceAppeared(const musicDevice::MusicDeviceId& mdId);
   // void musicDeviceDisappeared(const musicDevice::MusicDeviceId& mdId);
   void retriggerCallbacks();

   void loadFromFile();

private:
   instruments::InstrumentsRef m_rInstruments;
   musicDevice::MusicDeviceContainerRef m_rMusicDeviceContainer;
   using MapType = std::unordered_map<musicDevice::controller::EventIdExt, EventDestination>;
   util::DoubleBuffer<MapType> m_map;

   const musicDevice::description::sound::Parameter* parameterDescription(
       const EventDestination::Endpoint& endpoint, int paramIdx);

   CB_SIGNAL(GotConnected, const musicDevice::controller::EventIdExt&,
             const EventDestination&);
   CB_SIGNAL(GotErased, const musicDevice::controller::EventIdExt&);

   void printMap() const noexcept;

   static constexpr int ANY = -1;
};

}   // namespace base::eventRouter

#endif