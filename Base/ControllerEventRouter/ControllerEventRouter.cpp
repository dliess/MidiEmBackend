#include "ControllerEventRouter.h"

#include "ControllerHandler.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"

using namespace base::eventRouter;
using namespace base::musicDevice;

EventRouter::EventRouter(instruments::InstrumentsRef rInstruments,
                         MusicDeviceContainerRef rMusicDeviceContainer) :
    m_rInstruments(rInstruments), m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void EventRouter::loadFromFile()
{
   // TODO
   // m_data = m_settings.load<decltype(m_data)>(CONFIG_SECTION);
   // for (const auto& e : m_data) { emitEntry(e); }
}

void EventRouter::onControllerDevEventOccured(
    const util::Identifiable::UUID& uuid, const controller::Event& event)
{
   m_map.withRtLocked([this, &uuid, &event](const auto& map) {
      EventRouterRt(map, m_parameterCacheMap, m_rInstruments,
                    m_rMusicDeviceContainer)(uuid, event);
   });
}

void EventRouter::createConnection(const controller::EventIdExt& from,
                                   const EventDestination& to) noexcept
{
   EventDestination destination = to;
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
   m_map.withNonRtLocked([&](auto& map) { map[from] = destination; });
   emitGotConnected(from, destination);
}

void EventRouter::removeConnectionToDestination(
    const EventDestination& to) noexcept
{
   // TODO
}

void EventRouter::removeConnection(
    const controller::EventIdExt& eventIdExt) noexcept
{
   m_map.withNonRtLocked([&](auto& map) {
      auto it = map.find(eventIdExt);
      map.erase(it);
   });

   emitGotErased(eventIdExt);
}

void EventRouter::printMap() const noexcept
{
   // TODO
   /*
   for (const auto& e : m_map)
   {
      spdlog::info("{}", meta::serialize(e.first).dump().c_str());
   }
   */
}

void EventRouter::retriggerCallbacks()
{
   // TODO
   // for (auto& e : m_map) { emitGotConnected(e.first, e.second); }
}

const description::sound::Parameter* EventRouter::parameterDescription(
    const EventDestination::Endpoint& endpoint, int paramIdx)
{
   const description::sound::Parameter* ret{nullptr};
   mpark::visit(util::overload{[&](EventDestination::DrumKit& drumKit) {
                                  m_rInstruments.withKitInstrument(
                                      drumKit.uuid, [&](const auto& instr) {
                                         ret = instr.parameterDescription(
                                             drumKit.voiceIdx,
                                             drumKit.componentIdx, paramIdx);
                                      });
                               },
                               [&](EventDestination::Melodic& melodic) {
                                  m_rInstruments.withMelodicInstrument(
                                      melodic.uuid, [&](const auto& instr) {
                                         ret = instr.parameterDescription(
                                             melodic.componentIdx, paramIdx);
                                      });
                               },
                               [&](EventDestination::MusicDevice& musicDevice) {
                                  m_rMusicDeviceContainer.withSoundHandler(
                                      musicDevice.mdid, [&](const auto& sd) {
                                         ret = sd.parameterDescription(
                                             musicDevice.voiceIdx, paramIdx);
                                      });
                               },
                               [](auto&&) {}},
                endpoint);
   return ret;
}
