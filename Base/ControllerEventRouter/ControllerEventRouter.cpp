#include "ControllerEventRouter.h"

#include "ControllerEventRoutePersister.h"
#include "ControllerHandler.h"
#include "FilePersister.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"

using namespace base::eventRouter;
using namespace base::musicDevice;

EventRouter::EventRouter(instruments::InstrumentsRef rInstruments,
                         MusicDeviceContainerRef rMusicDeviceContainer) :
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer),
    m_persister(std::make_unique<util::FilePersister>("ControllerEventRouter",
                                                      "settings.json"))
{
   onGotConnected(
       [this](const musicDevice::controller::EventIdExt&,
              const EventDestination&) { m_persister.save(m_map.nonRt()); });
   onGotErased([this](const musicDevice::controller::EventIdExt&) {
      m_persister.save(m_map.nonRt());
   });

   try
   {
      auto data = m_persister.load();
      m_map.withNonRtLocked([&data](auto& nonRtData) { nonRtData = data; });
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

void EventRouter::createConnection(const controller::EventIdExt& from,
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
   }
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
   // printMap();
}

void EventRouter::removeConnectionToDestination(
    const EventDestination& to) noexcept
{
   // TODO
}

void EventRouter::removeConnection(
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
      map.erase(it);
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
               ret = sd.parameterDescription(
                  musicDevice.voiceIdx, paramIdx);
            });
      }
   END_SWITCH
   return ret;
}
