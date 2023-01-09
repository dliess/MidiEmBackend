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

inline controller::EventDestination::Endpoint toEndpoint(
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   switch (dest)
   {
      case ::capnzero::MidiEmRt::ControllerEventRouteDestination::DRUM_KIT:
      {
         return controller::EventDestination::DrumKit{util::deepCopy(destUUID),
                                                      voiceIdx};
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
                                    controller::EventDestination::Note{-1}});
}

void ControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx, controller::Note{note}, eventIdx, channelIdx};
   controller::EventDestination to;
   std::copy(soundDevUUID.begin(), soundDevUUID.end(), to.uuid.begin());
   to.voiceIdx   = voiceIdx;
   const auto it = m_rMusicDeviceContainer.find(to.uuid);
   if (it != m_rMusicDeviceContainer.end() && it->second->soundHandler)
   {
      const auto& paramDescr =
          it->second->description()->soundSection->parameterDescr(voiceIdx,
                                                                  parameterIdx);
      to.controlType = controller::EventDestination::Parameter{
          parameterIdx,
          static_cast<controller::ParameterDestination>(paramFunc),
          true,
          paramDescr.type == description::sound::Parameter::Type::List,
          paramDescr.getSourceResolution(),
          paramDescr.type ==
                  description::sound::Parameter::Type::ContinousBipolar
              ? 0.5f
              : 0.0f};
      m_rCtrlEventRouter.createConnection(from, to);
   }
   else
   {
      spdlog::error("Could not find destination uuid {} in music devices",
                    util::uuid2Str(to.uuid));
   }
}

void ControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx,
                   controller::WidgetCoord{widgetCoordY, widgetCoordX},
                   eventIdx, channelIdx};
   controller::EventDestination to;
   std::copy(soundDevUUID.begin(), soundDevUUID.end(), to.uuid.begin());
   to.voiceIdx    = voiceIdx;
   to.controlType = controller::EventDestination::Note{65};   // TODO
   m_rCtrlEventRouter.createConnection(from, to);
}

void ControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    ::capnzero::MidiEmRt::ControllerEventRouteDestination dest,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination paramFunc)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx,
                   controller::WidgetCoord{widgetCoordY, widgetCoordX},
                   eventIdx, channelIdx};
   controller::EventDestination to;
   std::copy(soundDevUUID.begin(), soundDevUUID.end(), to.uuid.begin());
   to.voiceIdx   = voiceIdx;
   const auto it = m_rMusicDeviceContainer.find(to.uuid);
   if (it != m_rMusicDeviceContainer.end() && it->second->soundHandler)
   {
      const auto& paramDescr =
          it->second->description()->soundSection->parameterDescr(voiceIdx,
                                                                  parameterIdx);
      to.controlType = controller::EventDestination::Parameter{
          parameterIdx,
          static_cast<controller::ParameterDestination>(paramFunc),
          true,
          paramDescr.type == description::sound::Parameter::Type::List,
          paramDescr.getSourceResolution(),
          paramDescr.type ==
                  description::sound::Parameter::Type::ContinousBipolar
              ? 0.5f
              : 0.0f};
      m_rCtrlEventRouter.createConnection(from, to);
   }
   else
   {
      spdlog::error("Could not find destination uuid {} in music devices",
                    util::uuid2Str(to.uuid));
   }
}

void ControllerEventRouterRpc::eraseConnectionForNotes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx, controller::Note{note}, eventIdx, channelIdx};
   m_rCtrlEventRouter.removeConnection(from);
}

void ControllerEventRouterRpc::eraseConnectionForWidget(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx)
{
   controller::EventIdExt from;
   std::copy(controllerUUID.begin(), controllerUUID.end(), from.uuid.begin());
   from.eventId = {widgetIdx,
                   controller::WidgetCoord{widgetCoordY, widgetCoordX},
                   eventIdx, channelIdx};
   m_rCtrlEventRouter.removeConnection(from);
}
