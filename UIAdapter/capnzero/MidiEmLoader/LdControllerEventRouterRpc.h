#ifndef LOADER_CONTROLLER_EVENT_ROUTER_RPC_H
#define LOADER_CONTROLLER_EVENT_ROUTER_RPC_H

#include "Identifiable.h"
#include "MidiEmLoaderControllerEventRouterRpcIf.h"

namespace base::eventRouter
{
class EventRouter;
}
namespace base::musicDevice::factory
{
class MusicDevices;
}
namespace base::instruments { class Instruments; }
namespace uiadapter::capnzero
{
class LdControllerEventRouterRpc
    : public ::capnzero::MidiEmLoader::ControllerEventRouterRpcIf
{
public:
   LdControllerEventRouterRpc(
       base::eventRouter::EventRouter& rCtrlEventRouter,
       base::instruments::Instruments& rInstruments,
       base::musicDevice::factory::MusicDevices& rMDFDataHolder) noexcept;
   void connectNotes2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
                           ::capnzero::Int16 eventIdx,
                           ::capnzero::Int16 channelIdx,
                           const ::capnzero::SpanCL<16>& destUUID,
                           ::capnzero::Int16 voiceIdx) override;
   void connectNotes2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterAttr paramFunc) override;
   void connectWidget2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                            ::capnzero::Int16 widgetIdx,
                            ::capnzero::Int16 widgetCoordX,
                            ::capnzero::Int16 widgetCoordY,
                            ::capnzero::Int16 eventIdx,
                            ::capnzero::Int16 channelIdx,
                            const ::capnzero::SpanCL<16>& destUUID,
                            ::capnzero::Int16 voiceIdx) override;
   void connectWidget2Parameter(
       const ::capnzero::SpanCL<16>& controllerUUID,
       ::capnzero::Int16 widgetIdx, ::capnzero::UInt32 widgetCoord,
       ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterAttr paramFunc) override;

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
       const ::capnzero::SpanCL<16>& destUUID,
       ::capnzero::Int16 voiceIdx) override;
   void eraseConnectionsToDestinationParameter(
       const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmLoader::SDParameterAttr paramFunc) override;

private:
   base::eventRouter::EventRouter& m_rCtrlEventRouter;
   base::instruments::Instruments& m_rInstruments;
   base::musicDevice::factory::MusicDevices& m_rMDFDataHolder;
};

}   // namespace uiadapter::capnzero
#endif
