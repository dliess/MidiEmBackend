#ifndef CONTROLLER_EVENT_ROUTER_RPC_H
#define CONTROLLER_EVENT_ROUTER_RPC_H

#include "MidiEmRtControllerEventRouterRpcIf.h"
#include "Identifiable.h"

namespace base::musicDevice::controller
{
class EventRouter;
}

namespace base::musicDevice
{
class MusicDeviceContainer;
}
namespace uiadapter::capnzero
{
class ControllerEventRouterRpc
    : public ::capnzero::MidiEmRt::ControllerEventRouterRpcIf
{
public:
   ControllerEventRouterRpc(
       base::musicDevice::controller::EventRouter& rCtrlEventRouter,
       base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept;
   void connectNotes2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
                           ::capnzero::Int16 eventIdx,
                           ::capnzero::Int16 channelIdx,
                           const ::capnzero::SpanCL<16>& soundDevUUID,
                           ::capnzero::Int16 voiceIdx) override;
   void connectNotes2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       const ::capnzero::SpanCL<16>& soundDevUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;
   void connectWidget2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                            ::capnzero::Int16 widgetIdx,
                            ::capnzero::Int16 widgetCoordX,
                            ::capnzero::Int16 widgetCoordY,
                            ::capnzero::Int16 eventIdx,
                            ::capnzero::Int16 channelIdx,
                            const ::capnzero::SpanCL<16>& soundDevUUID,
                            ::capnzero::Int16 voiceIdx) override;
   void connectWidget2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
       ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
       ::capnzero::Int16 channelIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;

    [[nodiscard]] bool isMelodic(const util::Identifiable::UUID& uuid) const noexcept;

private:
   base::musicDevice::controller::EventRouter& m_rCtrlEventRouter;
   base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
};

}   // namespace uiadapter::capnzero
#endif