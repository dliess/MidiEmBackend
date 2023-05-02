#ifndef CONTROLLER_EVENT_ROUTER_H
#define CONTROLLER_EVENT_ROUTER_H

#include "ControllerEventRouterData.h"
#include "ControllerEventRouterRt.h"
#include "CallbackSignal.h"
#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
#include "InstrumentsRef.h"
#include "MusicDeviceContainerRef.h"
#include "MusicDeviceDescription.h"
#include "DoubleBuffer.h"
#include "ControllerEventRoutePersister.h"


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

   CB_SIGNAL(GotConnected, const musicDevice::controller::EventIdExt&,
             const EventDestination&);
   CB_SIGNAL(GotErased, const musicDevice::controller::EventIdExt&);

private:
   instruments::InstrumentsRef m_rInstruments;
   musicDevice::MusicDeviceContainerRef m_rMusicDeviceContainer;
   util::DoubleBuffer<Data> m_map;
   ParameterCacheMap m_parameterCacheMap;
   Persister m_persister;

   const musicDevice::description::sound::Parameter* parameterDescription(
       const EventDestination::Endpoint& endpoint, int paramIdx);

   void printMap() const noexcept;

   static constexpr int ANY = -1;
};

}   // namespace base::eventRouter

#endif