#ifndef CONTROLLER_EVENT_ROUTER_RPC_H
#define CONTROLLER_EVENT_ROUTER_RPC_H

#include "MidiEmRtControllerEventRouterRpcIf.h"

namespace uiadapter::capnzero
{
class ControllerEventRouterRpc : public ::capnzero::MidiEmRt::ControllerEventRouterRpcIf
{
public:
   void connectNotes2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                                   ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
                                   const ::capnzero::SpanCL<16>& soundDevUUID,
                                   ::capnzero::Int16 voiceIdx) override;
   void connectNotes2Parameter(const ::capnzero::SpanCL<16>& controllerUUID,
                                       ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
                                       ::capnzero::Int16 eventIdx,
                                       const ::capnzero::SpanCL<16>& soundDevUUID,
                                       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
                                       ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;
   void connectWidget2Notes(const ::capnzero::SpanCL<16>& controllerUUID,
                                    ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
                                    ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
                                    const ::capnzero::SpanCL<16>& soundDevUUID,
                                    ::capnzero::Int16 voiceIdx) override;
   void connectWidget2Parameter(const ::capnzero::SpanCL<16>& controllerUUID,
                                        ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
                                        ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
                                        const ::capnzero::SpanCL<16>& soundDevUUID,
                                        ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
                                        ::capnzero::MidiEmRt::SDParameterDestination paramFunc) override;

};

}   // namespace uiadapter::capnzero
#endif