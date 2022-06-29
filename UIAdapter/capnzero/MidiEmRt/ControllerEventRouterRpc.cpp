#include "ControllerEventRouterRpc.h"

#include "ControllerEventRouter.h"
#include "MusicDeviceContainer.h"
#include "MusicDeviceDescription.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

ControllerEventRouterRpc::ControllerEventRouterRpc(
    controller::EventRouter& rCtrlEventRouter,
    base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
    m_rCtrlEventRouter(rCtrlEventRouter),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void ControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    const ::capnzero::SpanCL<16>& soundDevUUID, ::capnzero::Int16 voiceIdx)
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
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx,
                   controller::WidgetCoord{widgetCoordY, widgetCoordX},
                   eventIdx};
   controller::EventDestination to;
   std::copy(soundDevUUID.begin(), soundDevUUID.end(), to.uuid.begin());
   to.voiceIdx   = voiceIdx;
   to.endpoint = controller::EventDestination::Note { 65 }; // TODO
   m_rCtrlEventRouter.createConnection(from, to);
}

void ControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, const ::capnzero::SpanCL<16>& soundDevUUID,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx,
                   controller::WidgetCoord{widgetCoordY, widgetCoordX},
                   eventIdx};
   controller::EventDestination to;
   std::copy(soundDevUUID.begin(), soundDevUUID.end(), to.uuid.begin());
   to.voiceIdx   = voiceIdx;
   const auto it = m_rMusicDeviceContainer.find(to.uuid);
   if (it != m_rMusicDeviceContainer.end() && it->second->soundHandler)
   {
      const auto& paramDescr =
          it->second->description()->soundSection->parameterDescr(voiceIdx,
                                                                  parameterIdx);
      to.endpoint = controller::EventDestination::Parameter{
          parameterIdx, true,
          static_cast<controller::ParameterDestination>(paramFunc),
          paramDescr.type == description::sound::Parameter::Type::List,
          paramDescr.getSourceResolution()};
      m_rCtrlEventRouter.createConnection(from, to);
   }
   else
   {
      spdlog::error("Could not find destination uuid {} in music devices",
                    util::uuid2Str(to.uuid));
   }
}
