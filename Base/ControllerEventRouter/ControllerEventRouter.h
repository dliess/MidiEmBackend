#ifndef CONTROLLER_EVENT_ROUTER_H
#define CONTROLLER_EVENT_ROUTER_H

#include "CallbackSignal.h"
#include "ControllerEventDestination.h"
#include "ControllerEventRoutePersister.h"
#include "ControllerEventRouterData.h"
#include "ControllerEventRouterRt.h"
#include "ControllerEvents.h"
#include "DoubleBuffer.h"
#include "Refs/InstrumentsRef.h"
#include "MusicDeviceContainerRef.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "ErrorHandling.h"

namespace base::eventRouter
{
class EventRouter
{
public:
   EventRouter(instruments::InstrumentsRef rInstruments,
               musicDevice::MusicDeviceContainerRef rMusicDeviceContainer,
               musicDevice::factory::DataHolder& rMDFDataHolder);
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

   CB_SIGNAL(GotConnected, const musicDevice::controller::EventIdExt&,
             const EventDestination&);
   CB_SIGNAL(GotErased, const musicDevice::controller::EventIdExt&);

private:
   instruments::InstrumentsRef m_rInstruments;
   musicDevice::MusicDeviceContainerRef m_rMusicDeviceContainer;
   musicDevice::factory::DataHolder& m_rMDFDataHolder;
   util::DoubleBuffer<Data> m_map;
   Persister m_persister;
   LoaderData m_loaderData;

   Ret<const musicDevice::description::controller::Widget*> controlWidgetDescription(
       const musicDevice::controller::EventIdExt& evtId) const;

   Ret<const musicDevice::description::sound::Parameter*> parameterDescription(
       const EventDestination::Endpoint& endpoint, int paramIdx);

   Void _createConnection(const musicDevice::controller::EventIdExt& from,
                          const EventDestination& to) noexcept;
   void _removeConnection(
       const musicDevice::controller::EventIdExt& eventIdExt) noexcept;

   void printMap() const noexcept;
};

}   // namespace base::eventRouter

#endif
