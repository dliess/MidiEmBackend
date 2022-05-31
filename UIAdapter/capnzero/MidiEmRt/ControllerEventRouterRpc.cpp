#include "ControllerEventRouterRpc.h"
#include "ControllerEventRouter.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;


ControllerEventRouterRpc::ControllerEventRouterRpc(controller::EventRouter& rCtrlEventRouter) noexcept :
    m_rCtrlEventRouter(rCtrlEventRouter)
{

}

void ControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx)
{
    /*TODO*/
}
void ControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    const ::capnzero::SpanCL<16>& soundDevUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
    /*TODO*/
}
void ControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx)
{
    /*TODO*/
}
void ControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
    controller::EventIdExt from{/*TODO*/};
    controller::EventDestination to{/*TODO*/}; 
    m_rCtrlEventRouter.createConnection(from, to);
}
