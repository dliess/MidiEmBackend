#ifndef CONTROLLER_EVENT_ROUTER_RPC_H
#define CONTROLLER_EVENT_ROUTER_RPC_H

#include "Identifiable.h"
#include "MidiEmRtControllerEventRouterRpcIf.h"

namespace base::musicDevice::controller
{
class EventRouter;
}


namespace uiadapter::capnzero
{
class ControllerEventRouterRpc
    : public ::capnzero::MidiEmRt::ControllerEventRouterRpcIf
{
public:
   ControllerEventRouterRpc(
       base::musicDevice::controller::EventRouter& rCtrlEventRouter) noexcept;
   void connectNotes2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
                           ::capnzero::Int16 eventIdx,
                           ::capnzero::Int16 channelIdx,
                           ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
                           const ::capnzero::SpanCL<16>& destUUID,
                           ::capnzero::Int16 voiceIdx) override;
   void connectNotes2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;
   void connectWidget2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                            ::capnzero::Int16 widgetIdx,
                            ::capnzero::Int16 widgetCoordX,
                            ::capnzero::Int16 widgetCoordY,
                            ::capnzero::Int16 eventIdx,
                            ::capnzero::Int16 channelIdx,
                            ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
                            const ::capnzero::SpanCL<16>& destUUID,
                            ::capnzero::Int16 voiceIdx) override;
   void connectWidget2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
       ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
       ::capnzero::Int16 channelIdx,
       ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
       const ::capnzero::SpanCL<16>& destUUID,
       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 componentIdx,
       ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;

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

private:
   base::musicDevice::controller::EventRouter& m_rCtrlEventRouter;
};

}   // namespace uiadapter::capnzero
#endif