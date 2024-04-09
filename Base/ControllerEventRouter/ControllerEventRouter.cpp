#include "ControllerEventRouter.h"

#include "ControllerEventRoutePersister.h"
#include "ControllerHandler.h"
#include "FilePersister.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"
#include "ControllerEventRouterDataConverter.h"

using namespace base::eventRouter;
using namespace base::musicDevice;

inline
controller::EventIdExt changeNoteNumberToAnyIfDestIsMelodic(const controller::EventIdExt& from,
                                                            const EventDestination& to)
{
   static constexpr int ANY = -1;
   controller::EventIdExt source = from;
   if (auto note = dl::get_if<controller::Note>(&source.eventId.widgetCoord))
   {
      if (note->number != ANY &&
          dl::holds_alternative<EventDestination::Melodic>(
              to.endpoint))
      {
         note->number = ANY;
      }
   }
   return source;
}

EventRouter::EventRouter(instruments::InstrumentsRef rInstruments,
                         MusicDeviceContainerRef rMusicDeviceContainer,
                         musicDevice::factory::DataHolder& rMDFDataHolder) :
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer),
    m_rMDFDataHolder(rMDFDataHolder),
    m_persister(std::make_unique<util::FilePersister>("ControllerEventRouter",
                                                      "settings.json"))
{
   m_rMDFDataHolder.onMusicDeviceAdded([this](const musicDevice::MusicDevice* pMd){
      for(const auto& [eventId, eventDest] : m_loaderData)
      {
         if(eventId.uuid == pMd->deviceId())
         {
            const auto uuid = m_rMDFDataHolder.getUUIDByMdId(eventId.uuid);
            if(uuid)
            {
               _createConnection({*uuid, eventId.eventId}, eventDest);
            }
         }
      }
   });
   m_rMDFDataHolder.onMusicDeviceAboutToRemove([this](const musicDevice::MusicDevice* pMd){
      for(const auto& [eventId, _] : m_loaderData)
      {
         if(eventId.uuid == pMd->deviceId())
         {
            const auto uuid = m_rMDFDataHolder.getUUIDByMdId(eventId.uuid);
            if(uuid)
            {
               removeConnection({*uuid, eventId.eventId});
            }
         }
      }
   });

   try
   {
      m_loaderData = m_persister.load();
      const Data data = DataConverter(m_rMDFDataHolder).convertFromLoaded(m_loaderData);
      for(const auto& [evt, evtDest] : data)
      {
         _createConnection(evt, evtDest);
      }
   }
   catch (std::exception& e)
   {
      spdlog::error("Error loading ControllerEventRoute settings, its maybe "
                    "the first run: {}",
                    e.what());
   }
}

void EventRouter::onControllerDevEventOccured(
    const util::Identifiable::UUID& uuid, const controller::Event& event)
{
   m_map.withRtLocked([this, &uuid, &event](const auto& map) {
      EventRouterRt(map, m_rInstruments,
                    m_rMusicDeviceContainer)(uuid, event);
   });
}

void EventRouter::createConnection(const musicDevice::controller::EventIdExt& from,
                        const EventDestination& to) noexcept
{
   _createConnection(from, to);
   auto mdId = m_rMDFDataHolder.getMdIdByUUID(from.uuid);
   if(mdId)
   {
      const auto src = changeNoteNumberToAnyIfDestIsMelodic(from, to);
      m_loaderData.try_emplace({*mdId, src.eventId}, to);
      m_persister.save(m_loaderData);
   }
}

void EventRouter::removeConnection(
      const musicDevice::controller::EventIdExt& from) noexcept
{
   _removeConnection(from);
   auto mdId = m_rMDFDataHolder.getMdIdByUUID(from.uuid);
   if(mdId)
   {
      LoaderData::key_type key{*mdId, from.eventId};
      /*
      spdlog::info("Try to delete: {}" + nlohmann::json(key).dump());
      for(const auto& [theKey, element] : m_loaderData) {
         spdlog::info("Keys in container: {}" + nlohmann::json(theKey).dump());
      }
      */
      auto it = m_loaderData.find(key);
      if(it != m_loaderData.end())
      {
         m_loaderData.erase(it);
         m_persister.save(m_loaderData);
      }
   }
}

Void EventRouter::_createConnection(const controller::EventIdExt& from,
                                    const EventDestination& to) noexcept
{
   controller::EventIdExt source = changeNoteNumberToAnyIfDestIsMelodic(from, to);
   EventDestination destination  = to;

   if (auto param =
           dl::get_if<EventDestination::Parameter>(&destination.controlType))
   {
      return controlWidgetDescription(from).and_then(
          [&](const auto controllerWidgetDescr) {
            return parameterDescription(destination.endpoint, param->id)
                .map([&](const auto paramDesc) {
                   param->descriptionCache.isList =
                       (paramDesc->type == description::sound::Parameter::Type::List);
                   param->descriptionCache.resolution = paramDesc->getSourceResolution();
                   param->descriptionCache.zeroVal =
                       (paramDesc->type == description::sound::Parameter::Type::ContinousBipolar
                            ? 0.5f
                            : 0.0f);
                   const auto pressReleaseEvtIdx = description::controller::getDependentPressReleaseEventIdx(*controllerWidgetDescr);
                   if(pressReleaseEvtIdx)
                   {
                      controller::EventIdExt prEventId = from;
                      prEventId.eventId.eventId = pressReleaseEvtIdx.value();
                      const auto it = m_map.nonRt().find(prEventId);
                      if(it != m_map.nonRt().end() && it->second.endpoint == destination.endpoint)
                      {
                         param->descriptionCache.eventBound = true;
                      }
                   }
                });
          });
   }
   initRtCache(destination);
   m_map.withNonRtLocked(
       [&source, &destination](auto& map) { map[source] = destination; });

   // inc refcount of instrument
   SWITCH(destination.endpoint)
      CASE(EventDestination::DrumKit, drumKit)
      {
         m_rInstruments.incKitInstrumentRefCount(drumKit.uuid);
      },
      CASE(EventDestination::Melodic, melodic)
      {
         m_rInstruments.incMelodicInstrumentRefCount(melodic.uuid);
      },
      CASE(EventDestination::MusicDevice,_) {}
   END_SWITCH

   emitGotConnected(source, destination);
   return Void();
   //printMap();
}

void EventRouter::removeConnectionToDestination(
    const EventDestination& to) noexcept
{
   // TODO
}

void EventRouter::_removeConnection(
    const controller::EventIdExt& eventIdExt) noexcept
{
   if (auto iter = m_map.nonRt().find(eventIdExt); iter != m_map.nonRt().end())
   {
      // dec refcount of instrument
      SWITCH(iter->second.endpoint)
         CASE(EventDestination::DrumKit, drumKit)
         {
            m_rInstruments.decKitInstrumentRefCount(drumKit.uuid);
         },
         CASE(EventDestination::Melodic, melodic)
         {
            m_rInstruments.decMelodicInstrumentRefCount(melodic.uuid);
         },
         CASE(EventDestination::MusicDevice,_) {}
      END_SWITCH
   }
   bool erased = false;
   m_map.withNonRtLocked([&](auto& map) {
      auto it = map.find(eventIdExt);
      if(it != map.end())
      {
         erased = true;
         map.erase(it);
      }
   });
   if(erased) 
   {
      emitGotErased(eventIdExt);
   }
}

void EventRouter::printMap() const noexcept
{
   for (const auto& [from, to] : m_map.nonRt())
   {
      spdlog::info("{} -> {}", nlohmann::json(from).dump().c_str(),
                   nlohmann::json(to).dump().c_str());
   }
}

void EventRouter::retriggerCallbacks()
{
   for (const auto& [from, to] : m_map.nonRt()) { emitGotConnected(from, to); }
}

Ret<const description::controller::Widget*> EventRouter::controlWidgetDescription(
   const controller::EventIdExt& evtId) const
{
   return m_rMDFDataHolder.getDescription(evtId.uuid).and_then([&](auto desc) -> Ret<const description::controller::Widget*> {
      if(desc->controllerSection)
      {
         return safe_at(desc->controllerSection->widgets, 
                        evtId.eventId.widgetId).map(
                [](auto widget) { return widget; });
      }
      return tl::unexpected(Error::descriptionNotFound);
   });
}

Ret<const description::sound::Parameter*> EventRouter::parameterDescription(
    const EventDestination::Endpoint& endpoint, int paramIdx)
{
   return R_SWITCH(endpoint)
      CASE(EventDestination::DrumKit, drumKit) -> Ret<const description::sound::Parameter*>
      {
         return m_rInstruments.getParameterDescriptionOfKit(drumKit.uuid, drumKit.voiceIdx,
                                                            drumKit.componentIdx, paramIdx);
      },
      CASE(EventDestination::Melodic, melodic) -> Ret<const description::sound::Parameter*>
      {
         return m_rInstruments.getParameterDescriptionOfMelodic(melodic.uuid, 
                                                                melodic.componentIdx, paramIdx);
      },
      CASE(EventDestination::MusicDevice, musicDevice) -> Ret<const description::sound::Parameter*>
      {
         return m_rMusicDeviceContainer.getSoundHandler(musicDevice.mdid).map(
            [&](auto sd) {
               return &sd->parameterDescription(musicDevice.voiceIdx, paramIdx);
            });
      }
   R_END_SWITCH
}
