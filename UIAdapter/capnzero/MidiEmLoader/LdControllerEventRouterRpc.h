#ifndef LOADER_CONTROLLER_EVENT_ROUTER_RPC_H
#define LOADER_CONTROLLER_EVENT_ROUTER_RPC_H

#include "MidiEmLoaderControllerEventRouterRpcIf.h"
#include "Identifiable.h"

namespace base::musicDevice::factory { class Factory; }
namespace base::musicDevice::controller::loader { class EventRoutes; };

namespace uiadapter::capnzero
{
class LdControllerEventRouterRpc
    : public ::capnzero::MidiEmLoader::ControllerEventRouterRpcIf
{
public:
   LdControllerEventRouterRpc(
    base::musicDevice::factory::Factory& rMDFactory,
    base::musicDevice::controller::loader::EventRoutes& rEventRoutes
       ) noexcept;
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
       ::capnzero::Int16 paramFunc) override;
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
       ::capnzero::Int16 paramFunc) override;

    [[nodiscard]] bool isMelodic(const util::Identifiable::UUID& uuid) const noexcept;

private:
   base::musicDevice::factory::Factory& m_rMDFactory;
   base::musicDevice::controller::loader::EventRoutes& m_rEventRoutes;
};

}   // namespace uiadapter::capnzero
#endif