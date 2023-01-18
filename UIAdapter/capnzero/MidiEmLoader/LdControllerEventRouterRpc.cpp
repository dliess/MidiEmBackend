#include "LdControllerEventRouterRpc.h"

#include "ControllerEventRouter.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"

using namespace uiadapter::capnzero;
using namespace base;
using namespace base::musicDevice;

eventRouter::EventDestination::Endpoint toEndpoint(
    const base::musicDevice::factory::DataHolder &rMDFDataHolder,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx = eventRouter::EventDestination::DrumKit::NOT_SET)
{
   switch (dest)
   {
      case ::capnzero::MidiEmLoader::ControllerEventRouteDestination::DRUM_KIT:
      {
         return eventRouter::EventDestination::DrumKit{util::deepCopy(destUUID),
                                                       voiceIdx, componentIdx};
      }
      case ::capnzero::MidiEmLoader::ControllerEventRouteDestination::MELODIC:
      {
         return eventRouter::EventDestination::Melodic{util::deepCopy(destUUID),
                                                       voiceIdx};
      }
      case ::capnzero::MidiEmLoader::ControllerEventRouteDestination::
          MUSIC_DEVICE:
      {
         return eventRouter::EventDestination::MusicDevice{
             *rMDFDataHolder.getMdIdByUUID(destUUID), voiceIdx};
      }
      default:
      {
         assert(false);
         return eventRouter::EventDestination::DrumKit{};
      }
   }
}

LdControllerEventRouterRpc::LdControllerEventRouterRpc(
    base::eventRouter::EventRouter& rCtrlEventRouter,
    base::musicDevice::factory::DataHolder &rMDFDataHolder) noexcept :
    m_rCtrlEventRouter(rCtrlEventRouter), m_rMDFDataHolder(rMDFDataHolder)
{
}

void LdControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       eventRouter::EventDestination{toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx),
                                     eventRouter::EventDestination::Note{}});
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
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx, componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterDestination>(paramFunc)}});
}

void LdControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
            eventIdx, channelIdx}},
       eventRouter::EventDestination{toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx),
                                     eventRouter::EventDestination::Note{}});
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
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
            eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx, componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterDestination>(paramFunc)}});
}

void LdControllerEventRouterRpc::eraseConnectionForNotes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::Note{note}, eventIdx, channelIdx}});
}

void LdControllerEventRouterRpc::eraseConnectionForWidget(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
        eventIdx, channelIdx}});
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationNotes(
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.removeConnectionToDestination(
       eventRouter::EventDestination{toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx),
                                     eventRouter::EventDestination::Note{}});
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationParameter(
    ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.removeConnectionToDestination(
       eventRouter::EventDestination{
           toEndpoint(m_rMDFDataHolder, dest, destUUID, voiceIdx, componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterDestination>(paramFunc)}});
}