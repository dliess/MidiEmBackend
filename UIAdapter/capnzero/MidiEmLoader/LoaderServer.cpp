#include "LoaderServer.h"

#include "ControllerEventRouter.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // meta::serialize
#include "LdControllerEventRouterRpc.h"
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

auto unpackEndpoint(
    const base::musicDevice::factory::DataHolder &rMDFDataHolder,
    const base::eventRouter::EventDestination::Endpoint &endpoint)
{
   using RetType =
       std::tuple<::capnzero::MidiEmLoader::InstrumentType,
                  ::capnzero::Data<16>, ::capnzero::Int16, ::capnzero::Int16>;
   return mpark::visit(
       util::overload{
           [](const base::eventRouter::EventDestination::DrumKit &drumKit)
               -> RetType {
              return RetType{::capnzero::MidiEmLoader::
                                 InstrumentType::DRUM_KIT,
                             drumKit.uuid, drumKit.voiceIdx,
                             drumKit.componentIdx};
           },
           [](const base::eventRouter::EventDestination::Melodic &melodic)
               -> RetType {
              return RetType{::capnzero::MidiEmLoader::
                                 InstrumentType::MELODIC,
                             melodic.uuid, 0, melodic.componentIdx};
           },
           [&](const base::eventRouter::EventDestination::MusicDevice
                   &musicDevice) -> RetType {
              return RetType{::capnzero::MidiEmLoader::
                                 InstrumentType::MUSIC_DEVICE,
                             *rMDFDataHolder.getUUIDByMdId(musicDevice.mdid),
                             musicDevice.voiceIdx, 0};
           }},
       endpoint);
}

LoaderServer::LoaderServer(zmq::context_t &rZmqContext,
                           const std::string &rpcBindAddr,
                           const std::string &signalBindAddr,
                           base::musicDevice::factory::Factory &rMDFactory,
                           base::instruments::Instruments &rInstruments,
                           base::eventRouter::EventRouter &rCtrlEventRouter) :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(
        rZmqContext, rpcBindAddr, signalBindAddr,
        std::make_unique<LoaderRpc>(signals(), rInstruments, rMDFactory,
                                    rCtrlEventRouter),
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<LdControllerEventRouterRpc>(rCtrlEventRouter,
                                                     rMDFactory.dataHolder()))
{
   rCtrlEventRouter.onGotConnected([this, &rMDFactory](
                                       const base::musicDevice::controller::
                                           EventIdExt &from,
                                       const base::eventRouter::EventDestination
                                           &to) {
      auto [e_type, e_uuid, e_voiceIdx, e_comIdx] =
          unpackEndpoint(rMDFactory.dataHolder(), to.endpoint);
      mpark::visit(
          util::overload{
              [](const mpark::monostate
                     &) { /* TODO */
                          spdlog::error(
                              "Unhandled path in controller-event-connection");
              },
              [&](const base::musicDevice::controller::WidgetCoord
                      &widgetCoord) {
                 mpark::visit(
                     util::overload{
                         [&](const base::eventRouter::EventDestination::Note
                                 &note) {
                            signals()
                                .ControllerEventRouter__connectedWidget2Notes(
                                    from.uuid, from.eventId.widgetId,
                                    widgetCoord.col, widgetCoord.row,
                                    from.eventId.eventId,
                                    from.eventId.channelId, e_type, e_uuid,
                                    e_voiceIdx);
                         },
                         [&](const base::eventRouter::EventDestination::
                                 Parameter &parameter) {
                            signals()
                                .ControllerEventRouter__connectedWidget2Parameter(
                                    from.uuid, from.eventId.widgetId,
                                    widgetCoord.col, widgetCoord.row,
                                    from.eventId.eventId,
                                    from.eventId.channelId, e_type, e_uuid,
                                    e_voiceIdx, e_comIdx, parameter.id,
                                    static_cast<::capnzero::MidiEmLoader::
                                                    SDParameterDestination>(
                                        parameter.parameterDestination));
                         },
                         [](auto &&) {
                            spdlog::error("Unhandled path in "
                                          "controller-event-connection");
                         }},
                     to.controlType);
              },
              [&](const base::musicDevice::controller::Note &note) {
                 mpark::visit(
                     util::overload{
                         [&](const base::eventRouter::EventDestination::Note
                                 &) {
                            signals()
                                .ControllerEventRouter__connectedNotes2Notes(
                                    from.uuid, from.eventId.widgetId,
                                    note.number, from.eventId.eventId,
                                    from.eventId.channelId, e_type, e_uuid,
                                    e_voiceIdx);
                         },
                         [&](const base::eventRouter::EventDestination::
                                 Parameter &parameter) {
                            signals()
                                .ControllerEventRouter__connectedNotes2Parameter(
                                    from.uuid, from.eventId.widgetId,
                                    note.number, from.eventId.eventId,
                                    from.eventId.channelId, e_type, e_uuid,
                                    e_voiceIdx, e_comIdx, parameter.id,
                                    static_cast<::capnzero::MidiEmLoader::
                                                    SDParameterDestination>(
                                        parameter.parameterDestination));
                         },
                         [](auto &&) {
                            spdlog::error("Unhandled path in "
                                          "controller-event-connection");
                         }},
                     to.controlType);
              },
          },
          from.eventId.widgetCoord);
   });
   rCtrlEventRouter.onGotErased([this](const base::musicDevice::controller::
                                           EventIdExt &from) {
      mpark::visit(
          util::overload{
              [](const mpark::monostate
                     &) { /* TODO */
                          spdlog::error(
                              "Unhandled path in controller-event-connection");
              },
              [&](const base::musicDevice::controller::WidgetCoord
                      &widgetCoord) {
                 signals().ControllerEventRouter__erasedConnectionForWidget(
                     from.uuid, from.eventId.widgetId, widgetCoord.col,
                     widgetCoord.row, from.eventId.eventId,
                     from.eventId.channelId);
              },
              [&](const base::musicDevice::controller::Note &note) {
                 signals().ControllerEventRouter__erasedConnectionForNotes(
                     from.uuid, from.eventId.widgetId, note.number,
                     from.eventId.eventId, from.eventId.channelId);
              },
          },
          from.eventId.widgetCoord);
   });
}
