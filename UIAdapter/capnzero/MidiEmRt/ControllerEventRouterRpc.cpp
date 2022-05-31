#include "ControllerEventRouterRpc.h"

using namespace uiadapter::capnzero;

void ControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx)
{
}
void ControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    const ::capnzero::SpanCL<16>& soundDevUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
}
void ControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx)
{
}
void ControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
}
