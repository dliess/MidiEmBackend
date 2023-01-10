#include "ControllerEventRouterRpc.h"

#include "ControllerEventRouter.h"
#include "MusicDeviceDescription.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

ControllerEventRouterRpc::ControllerEventRouterRpc(
    controller::EventRouter& rCtrlEventRouter,
    base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
    m_rCtrlEventRouter(rCtrlEventRouter)
{
}

controller::EventDestination::Endpoint toEndpoint(
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx =
        controller::EventDestination::DrumKit::NOT_SET)
{
   switch (dest)
   {
      case ::capnzero::MidiEmRt::ControllerEventRouteDestination::DRUM_KIT:
      {
         return controller::EventDestination::DrumKit{util::deepCopy(destUUID),
                                                      voiceIdx, componentIdx};
      }
      case ::capnzero::MidiEmRt::ControllerEventRouteDestination::MELODIC:
      {
         return controller::EventDestination::Melodic{util::deepCopy(destUUID),
                                                      voiceIdx};
      }
      case ::capnzero::MidiEmRt::ControllerEventRouteDestination::MUSIC_DEVICE:
      {
         return controller::EventDestination::MusicDevice{
             util::deepCopy(destUUID), voiceIdx};
      }
   }
}

void ControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       controller::EventDestination{toEndpoint(dest, destUUID, voiceIdx),
                                    controller::EventDestination::Note{}});
}

void ControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       controller::EventDestination{
           toEndpoint(dest, destUUID, voiceIdx, componentIdx),
           controller::EventDestination::Parameter{
               parameterIdx,
               static_cast<controller::ParameterDestination>(paramFunc)}});
}

void ControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
            eventIdx, channelIdx}},
       controller::EventDestination{toEndpoint(dest, destUUID, voiceIdx),
                                    controller::EventDestination::Note{}});
}

void ControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
            eventIdx, channelIdx}},
       controller::EventDestination{
           toEndpoint(dest, destUUID, voiceIdx, componentIdx),
           controller::EventDestination::Parameter{
               parameterIdx,
               static_cast<controller::ParameterDestination>(paramFunc)}});
}

void ControllerEventRouterRpc::eraseConnectionForNotes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::Note{note}, eventIdx, channelIdx}});
}

void ControllerEventRouterRpc::eraseConnectionForWidget(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
        eventIdx, channelIdx}});
}
