#ifndef LOADER_CONTROLLER_EVENT_ROUTER_RPC_H
#define LOADER_CONTROLLER_EVENT_ROUTER_RPC_H

#include "Identifiable.h"
#include "MidiEmLoaderControllerEventRouterRpcIf.h"

namespace base::eventRouter
{
class EventRouter;
}
namespace base::musicDevice { class MusicDeviceContainer; }

namespace uiadapter::capnzero
{
class LdControllerEventRouterRpc
    : public ::capnzero::MidiEmLoader::ControllerEventRouterRpcIf
{
public:
   LdControllerEventRouterRpc(
       base::eventRouter::EventRouter& rCtrlEventRouter,
       base::musicDevice::MusicDeviceContainer& rMdContainer) noexcept;
   void connectNotes2Notes(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID,
       ::capnzero::Int16 voiceIdx) override;
   void connectNotes2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterDestination paramFunc) override;
   void connectWidget2Notes(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
       ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
       ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID,
       ::capnzero::Int16 voiceIdx) override;
   void connectWidget2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
       ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
       ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterDestination paramFunc) override;

   void eraseConnectionForNotes(const ::capnzero::SpanCL<16>& controllerUUID,
                                ::capnzero::Int16 widgetIdx,
                                ::capnzero::Int16 note,
                                ::capnzero::Int16 eventIdx,
                                ::capnzero::Int16 channelIdx) override;
   void eraseConnectionForWidget(const ::capnzero::SpanCL<16>& controllerUUID,
                                 ::capnzero::Int16 widgetIdx,
                                 ::capnzero::Int16 widgetCoordX,
                                 ::capnzero::Int16 widgetCoordY,
                                 ::capnzero::Int16 eventIdx,
                                 ::capnzero::Int16 channelIdx) override;

   void eraseConnectionsToDestinationNotes(
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID,
       ::capnzero::Int16 voiceIdx) override;
   void eraseConnectionsToDestinationParameter(
       ::capnzero::MidiEmLoader::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterDestination paramFunc) override;

private:
   base::eventRouter::EventRouter& m_rCtrlEventRouter;
   base::musicDevice::MusicDeviceContainer& m_rMdContainer;
};

}   // namespace uiadapter::capnzero
#endif