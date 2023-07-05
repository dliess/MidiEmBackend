#include "LoaderServer.h"

#include "ControllerEventRouter.h"
#include "Instruments.h"
#include "MusicDevicesRpc.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // nlohmann::json
#include "LdControllerEventRouterRpc.h"
#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base;

static_assert(int(::capnzero::MidiEmLoader::SDParameterAttr::PARAMETER) == 
              int(base::musicDevice::sound::ParameterAttr::Commanded));
static_assert(int(::capnzero::MidiEmLoader::SDParameterAttr::L_F_O_FREQ) == 
              int(base::musicDevice::sound::ParameterAttr::LfoFrequency));
static_assert(int(::capnzero::MidiEmLoader::SDParameterAttr::L_F_O_AMP) == 
              int(base::musicDevice::sound::ParameterAttr::LfoAmplitude));
static_assert(int(::capnzero::MidiEmLoader::SDParameterAttr::L_F_O_WAVEFORM) == 
              int(base::musicDevice::sound::ParameterAttr::LfoWaveform));
static_assert(int(::capnzero::MidiEmLoader::SDParameterAttr::L_F_O_MULTIPLIER_EXP) == 
              int(base::musicDevice::sound::ParameterAttr::LfoMultiplierExp));

auto unpackEndpoint(
    const musicDevice::factory::DataHolder &rMDFDataHolder,
    const eventRouter::EventDestination::Endpoint &endpoint)
{
    static constexpr int UNUSED = -1;
   using RetType =
       std::tuple<::capnzero::Data<16>, ::capnzero::Int16, ::capnzero::Int16>;
   return R_SWITCH(endpoint)
        FCASE(eventRouter::EventDestination::DrumKit, drumKit)
        {
            return RetType{drumKit.uuid, drumKit.voiceIdx,
                            drumKit.componentIdx};
        },
        FCASE(eventRouter::EventDestination::Melodic, melodic) 
        {
            return RetType{melodic.uuid, UNUSED, melodic.componentIdx};
        },
        FCASE(eventRouter::EventDestination::MusicDevice, musicDevice) 
        {
            return RetType{*rMDFDataHolder.getUUIDByMdId(musicDevice.mdid),
                            musicDevice.voiceIdx, UNUSED};
        }
    R_END_SWITCH
}

uint32_t packWidgetCoordTo32(int x, int y) {
    return (x << 16) | (y & 0x0000FFFF);
}

LoaderServer::LoaderServer(zmq::context_t &rZmqContext,
                           const std::string &rpcBindAddr,
                           const std::string &signalBindAddr,
                           musicDevice::factory::Factory &rMDFactory,
                           instruments::Instruments &rInstruments,
                           eventRouter::EventRouter &rCtrlEventRouter) :
    ::capnzero::MidiEmLoader::MidiEmLoaderServer(
        rZmqContext, rpcBindAddr, signalBindAddr,
        std::make_unique<LoaderRpc>(signals(), rInstruments, rMDFactory,
                                    rCtrlEventRouter),
        std::make_unique<MusicDevicesRpc>(rMDFactory.dataHolder()),
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<LdControllerEventRouterRpc>(rCtrlEventRouter,
                                                     rInstruments,
                                                     rMDFactory.dataHolder()))
{
   rCtrlEventRouter.onGotConnected([this, &rMDFactory](
                                       const musicDevice::controller::
                                           EventIdExt &from,
                                       const eventRouter::EventDestination
                                           &to) {
      auto [e_uuid, e_voiceIdx, e_comIdx] =
          unpackEndpoint(rMDFactory.dataHolder(), to.endpoint);
      SWITCH(from.eventId.widgetCoord)
            CASE_MONOSTATE 
            { /* TODO */
                        spdlog::error(
                            "Unhandled path in controller-event-connection");
            },
            CASE(musicDevice::controller::WidgetCoord, widgetCoord)
            {
                SWITCH(to.controlType)
                    CASE(eventRouter::EventDestination::Note, _)
                    {
                    signals()
                        .ControllerEventRouter__connectedWidget2Notes(
                            from.uuid, from.eventId.widgetId,
                            widgetCoord.col, widgetCoord.row,
                            from.eventId.eventId,
                            from.eventId.channelId, e_uuid,
                            e_voiceIdx);
                    },
                    CASE(eventRouter::EventDestination::Parameter, parameter) 
                    {
                    signals()
                        .ControllerEventRouter__connectedWidget2Parameter(
                            from.uuid, from.eventId.widgetId,
                            packWidgetCoordTo32(widgetCoord.col, widgetCoord.row),
                            from.eventId.eventId,
                            from.eventId.channelId, e_uuid,
                            e_voiceIdx, e_comIdx, parameter.id,
                            static_cast<::capnzero::MidiEmLoader::
                                            SDParameterAttr>(
                                parameter.parameterAttr));
                    }
                END_SWITCH
            },
            CASE(musicDevice::controller::Note, note)
            {
                SWITCH(to.controlType)
                    CASE(eventRouter::EventDestination::Note, _)
                    {
                    signals()
                        .ControllerEventRouter__connectedNotes2Notes(
                            from.uuid, from.eventId.widgetId,
                            note.number, from.eventId.eventId,
                            from.eventId.channelId, e_uuid,
                            e_voiceIdx);
                    },
                    CASE(eventRouter::EventDestination::Parameter, parameter)
                    {
                    signals()
                        .ControllerEventRouter__connectedNotes2Parameter(
                            from.uuid, from.eventId.widgetId,
                            note.number, from.eventId.eventId,
                            from.eventId.channelId, e_uuid,
                            e_voiceIdx, e_comIdx, parameter.id,
                            static_cast<::capnzero::MidiEmLoader::
                                            SDParameterAttr>(
                                parameter.parameterAttr));
                    }
                END_SWITCH
            }
        END_SWITCH
   });
   rCtrlEventRouter.onGotErased([this](const musicDevice::controller::
                                           EventIdExt &from) {
      SWITCH(from.eventId.widgetCoord)     
        CASE_MONOSTATE
        { /* TODO */
            spdlog::error(
                "Unhandled path in controller-event-connection");
        },
        FCASE(musicDevice::controller::WidgetCoord, widgetCoord)
        {
            signals().ControllerEventRouter__erasedConnectionForWidget(
                from.uuid, from.eventId.widgetId, widgetCoord.col,
                widgetCoord.row, from.eventId.eventId,
                from.eventId.channelId);
        },
        FCASE(musicDevice::controller::Note, note)
        {
            signals().ControllerEventRouter__erasedConnectionForNotes(
                from.uuid, from.eventId.widgetId, note.number,
                from.eventId.eventId, from.eventId.channelId);
        }
        END_SWITCH
   });
}
