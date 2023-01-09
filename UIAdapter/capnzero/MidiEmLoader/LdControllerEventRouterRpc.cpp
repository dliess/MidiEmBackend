#include "LdControllerEventRouterRpc.h"

#include "ControllerEventRouterLoader.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

LdControllerEventRouterRpc::LdControllerEventRouterRpc(
    base::musicDevice::factory::Factory& rMDFactory,
    base::musicDevice::controller::loader::EventRoutes& rEventRoutes) noexcept :
    m_rMDFactory(rMDFactory), m_rEventRoutes(rEventRoutes)
{
}

void LdControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   const auto pControllerUUID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   const auto pSoundDevID = m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   if (note >= 0 && isMelodic(destUUID))
   {
      note = -1;
   }
   m_rEventRoutes.connectNotes2Notes(*pControllerUUID, widgetIdx, note,
                                     eventIdx, channelIdx, *pSoundDevID,
                                     voiceIdx);
}

bool LdControllerEventRouterRpc::isMelodic(
    util::Identifiable::UUIDView uuid) const noexcept
{
   const auto pDescr = m_rMDFactory.dataHolder().getDescription(uuid);
   if (pDescr)
   {
      return pDescr->soundSection->defaultInstrumentType ==
                 base::musicDevice::description::sound::Section::
                     DefaultInstrumentType::InstrumentPerVoice ||
             pDescr->soundSection->defaultInstrumentType ==
                 base::musicDevice::description::sound::Section::
                     DefaultInstrumentType::OnePolyphonicInstrument;
   }
   return false;
}

void LdControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   const auto pControllerUUID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   const auto pSoundDevUUID = m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.connectNotes2Parameter(
       *pControllerUUID, widgetIdx, note, eventIdx, channelIdx, *pSoundDevUUID,
       voiceIdx, parameterIdx,
       static_cast<controller::ParameterDestination>(paramFunc));
}

void LdControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   const auto pControllerUUID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   const auto pSoundDevUUID =
       m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.connectWidget2Notes(*pControllerUUID, widgetIdx, widgetCoordX,
                                      widgetCoordY, eventIdx, channelIdx,
                                      *pSoundDevUUID, voiceIdx);
}

void LdControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   const auto pControllerUUID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   const auto pSoundDevUUID =
       m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.connectWidget2Parameter(
       *pControllerUUID, widgetIdx, widgetCoordX, widgetCoordY, eventIdx,
       channelIdx, *pSoundDevUUID, voiceIdx, parameterIdx,
       static_cast<controller::ParameterDestination>(paramFunc));
}

void LdControllerEventRouterRpc::eraseConnectionForNotes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx)
{
   const auto pControllerUUID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerUUID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.eraseConnectionForNotes(*pControllerUUID, widgetIdx, note,
                                          eventIdx, channelIdx);
}

void LdControllerEventRouterRpc::eraseConnectionForWidget(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx)
{
   const auto pControllerMDID =
       m_rMDFactory.dataHolder().musicDeviceId(controllerUUID);
   if (!pControllerMDID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.eraseConnectionForWidget(*pControllerMDID, widgetIdx,
                                           widgetCoordX, widgetCoordY, eventIdx,
                                           channelIdx);
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationNotes(
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   const auto pSoundDevMDID =
       m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevMDID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.eraseConnectionsToDestinationNotes(*pSoundDevMDID, voiceIdx);
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationParameter(
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   const auto pSoundDevMDID =
       m_rMDFactory.dataHolder().musicDeviceId(destUUID);
   if (!pSoundDevMDID)
   {
      spdlog::error("No mdId found for uuid");
      return;
   }
   m_rEventRoutes.eraseConnectionsToDestinationParameter(
       *pSoundDevMDID, voiceIdx, parameterIdx,
       static_cast<controller::ParameterDestination>(paramFunc));
}
