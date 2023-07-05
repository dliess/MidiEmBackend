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
      m_loaderData.try_emplace({*mdId, from.eventId}, to);
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
      auto it = m_loaderData.find({*mdId, from.eventId});
      m_loaderData.erase(it);
      m_persister.save(m_loaderData);
   }
}

void EventRouter::_createConnection(const controller::EventIdExt& from,
                                   const EventDestination& to) noexcept
{
   controller::EventIdExt source = from;
   EventDestination destination  = to;
   if (auto note = mpark::get_if<controller::Note>(&source.eventId.widgetCoord))
   {
      if (note->number != ANY &&
          mpark::holds_alternative<EventDestination::Melodic>(
              destination.endpoint))
      {
         note->number = ANY;
      }
   }

   if (auto param =
           mpark::get_if<EventDestination::Parameter>(&destination.controlType))
   {
      auto controllerWidgetDescr = controlWidgetDescription(from);
      if (!controllerWidgetDescr)
      {
         spdlog::error("Error getting controller widget description");
         return;
      }
      auto desc = parameterDescription(destination.endpoint, param->id);
      if (!desc)
      {
         spdlog::error("Error getting parameter description");
         return;
      }
      param->descriptionCache.isList =
          (desc->type == description::sound::Parameter::Type::List);
      param->descriptionCache.resolution = desc->getSourceResolution();
      param->descriptionCache.zeroVal =
          (desc->type == description::sound::Parameter::Type::ContinousBipolar
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
   m_map.withNonRtLocked([&](auto& map) {
      auto it = map.find(eventIdExt);
      if(it != map.end())
      {
         map.erase(it);
      }
   });

   emitGotErased(eventIdExt);
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

const description::controller::Widget* EventRouter::controlWidgetDescription(
   const controller::EventIdExt& evtId) const
{
   const auto description = m_rMDFDataHolder.getDescription(evtId.uuid);
   if(description && description->controllerSection)
   {
      return &description->controllerSection->widgets.at(evtId.eventId.widgetId);
   }
   return nullptr;
}

const description::sound::Parameter* EventRouter::parameterDescription(
    const EventDestination::Endpoint& endpoint, int paramIdx)
{
   const description::sound::Parameter* ret{nullptr};
   SWITCH(endpoint)
      CASE(EventDestination::DrumKit, drumKit)
      {
         m_rInstruments.withKitInstrumentRt(
            drumKit.uuid, [&](const auto& instr) {
               ret = instr.parameterDescription(
                  drumKit.voiceIdx,
                  drumKit.componentIdx, paramIdx);
            });
      },
      CASE(EventDestination::Melodic, melodic)
      {
         m_rInstruments.withMelodicInstrumentRt(
            melodic.uuid, [&](const auto& instr) {
               ret = instr.parameterDescription(
                  melodic.componentIdx, paramIdx);
            });
      },
      CASE(EventDestination::MusicDevice, musicDevice)
      {
         m_rMusicDeviceContainer.withSoundHandler(
            musicDevice.mdid, [&](const auto& sd) {
               ret = &sd.parameterDescription(
                  musicDevice.voiceIdx, paramIdx);
            });
      }
   END_SWITCH
   return ret;
}
