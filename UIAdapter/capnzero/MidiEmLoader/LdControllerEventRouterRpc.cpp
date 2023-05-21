#include "LdControllerEventRouterRpc.h"

#include "ControllerEventRouter.h"
#include "Instruments.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactoryDataHolder.h"

using namespace uiadapter::capnzero;
using namespace base;
using namespace base::musicDevice;

eventRouter::EventDestination::Endpoint toEndpoint(
    const base::instruments::Instruments& rInstrumnets,
    const base::musicDevice::factory::DataHolder& rMDFDataHolder,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx =
        eventRouter::EventDestination::DrumKit::NOT_SET)
{
   if (rInstrumnets.hasKitInstrument(destUUID))
   {
      return eventRouter::EventDestination::DrumKit{util::deepCopy(destUUID),
                                                    voiceIdx, componentIdx};
   }
   if (rInstrumnets.hasMelodicInstrument(destUUID))
   {
      return eventRouter::EventDestination::Melodic{util::deepCopy(destUUID),
                                                    voiceIdx};
   }
   auto mdId = rMDFDataHolder.getMdIdByUUID(destUUID);
   if (mdId)
   {
      return eventRouter::EventDestination::MusicDevice{*mdId, voiceIdx};
   }
   assert(false);
   return eventRouter::EventDestination::DrumKit{};
}

// Oh my, this is only here to workaround the 10 arguments limit 
// of qt webchannel rpc-s
std::pair<int, int> extractWidgetCoordsXY(::capnzero::UInt32 widgetCoord)
{
   return std::make_pair((widgetCoord >> 16) & 0x0000FFFF,
                         widgetCoord & 0x0000FFFF);
}

LdControllerEventRouterRpc::LdControllerEventRouterRpc(
    base::eventRouter::EventRouter& rCtrlEventRouter,
    base::instruments::Instruments& rInstruments,
    base::musicDevice::factory::DataHolder& rMDFDataHolder) noexcept :
    m_rCtrlEventRouter(rCtrlEventRouter),
    m_rInstruments(rInstruments),
    m_rMDFDataHolder(rMDFDataHolder)
{
}

void LdControllerEventRouterRpc::connectNotes2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx, const ::capnzero::SpanCL<16>& destUUID,
    ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx),
           eventRouter::EventDestination::Note{}});
}

void LdControllerEventRouterRpc::connectNotes2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx, const ::capnzero::SpanCL<16>& destUUID,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 componentIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::Note{note}, eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx,
                      componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterAttr>(paramFunc)}});
}

void LdControllerEventRouterRpc::connectWidget2Notes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
            eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx),
           eventRouter::EventDestination::Note{}});
}

void LdControllerEventRouterRpc::connectWidget2Parameter(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::UInt32 widgetCoord, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx, const ::capnzero::SpanCL<16>& destUUID,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 componentIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.createConnection(
       controller::EventIdExt{
           util::deepCopy(controllerUUID),
           {widgetIdx,
            controller::WidgetCoord{extractWidgetCoordsXY(widgetCoord).first,
                                    extractWidgetCoordsXY(widgetCoord).second},
            eventIdx, channelIdx}},
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx,
                      componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterAttr>(paramFunc)}});
}

void LdControllerEventRouterRpc::eraseConnectionForNotes(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 note, ::capnzero::Int16 eventIdx,
    ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::Note{note}, eventIdx, channelIdx}});
}

void LdControllerEventRouterRpc::eraseConnectionForWidget(
    const ::capnzero::SpanCL<16>& controllerUUID, ::capnzero::Int16 widgetIdx,
    ::capnzero::Int16 widgetCoordX, ::capnzero::Int16 widgetCoordY,
    ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx)
{
   m_rCtrlEventRouter.removeConnection(controller::EventIdExt{
       util::deepCopy(controllerUUID),
       {widgetIdx, controller::WidgetCoord{widgetCoordY, widgetCoordX},
        eventIdx, channelIdx}});
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationNotes(
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx)
{
   m_rCtrlEventRouter.removeConnectionToDestination(
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx),
           eventRouter::EventDestination::Note{}});
}

void LdControllerEventRouterRpc::eraseConnectionsToDestinationParameter(
    const ::capnzero::SpanCL<16>& destUUID, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmLoader::SDParameterDestination paramFunc)
{
   m_rCtrlEventRouter.removeConnectionToDestination(
       eventRouter::EventDestination{
           toEndpoint(m_rInstruments, m_rMDFDataHolder, destUUID, voiceIdx,
                      componentIdx),
           eventRouter::EventDestination::Parameter{
               parameterIdx,
               static_cast<eventRouter::ParameterAttr>(paramFunc)}});
}