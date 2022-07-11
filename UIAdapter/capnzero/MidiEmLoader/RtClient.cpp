#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(
    zmq::context_t& rZmqContext, const std::string& rtServerRpcAddr,
    const std::string& rtServerSignalAddr, LoaderServer::Signals& rSignals,
    base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext, rtServerRpcAddr),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext, rtServerSignalAddr)
{
   onSoundDevicesPresetChanged(
       [this, &rMDFactory](const ::capnzero::TextView& deviceName,
                           ::capnzero::Int16 engineIdx,
                           const ::capnzero::TextView& presetName) {
          const sound::preset::Id presetId(
              {std::string(deviceName), engineIdx, std::string(presetName)});
          rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
       });

   onMusicDevicesDeviceAdded(
       [&rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid,
                                const ::capnzero::TextView& type,
                                const ::capnzero::TextView& port,
                                const ::capnzero::TextView& mediumId,
                                ::capnzero::Int16 midiVoiceOffset) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().addUuid2MdId(
              uuidData, MusicDeviceId{std::string(type), std::string(port)});
       });
   onMusicDevicesDeviceRemoved(
       [&rMDFactory, &rSignals](const ::capnzero::SpanCL<16>& uuid) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().removeEntryForUuid(uuidData);
       });

   onAbletonLinkEnabledChanged([](::capnzero::Bool val) {
      // TODO: save it to file and load
   });
   onAbletonLinkOffsetTimeUsChanged([](::capnzero::Int32 val) {
      // TODO: save it to file and load
   });

   onControllerEventRouterConnectedNotes2Notes(
       [this, &rMDFactory](
           const ::capnzero::SpanCL<16>& controllerUUID,
           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
           ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
           const ::capnzero::SpanCL<16>& soundDevUUID,
           ::capnzero::Int16 voiceIdx) {
          util::Identifiable::UUID _controllerUUID;
          std::copy(controllerUUID.begin(), controllerUUID.end(),
                    _controllerUUID.begin());
          const auto pControllerUUID =
              rMDFactory.dataHolder().musicDeviceId(_controllerUUID);
          if (!pControllerUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          util::Identifiable::UUID _soundDevUUID;
          std::copy(soundDevUUID.begin(), soundDevUUID.end(),
                    _soundDevUUID.begin());
          const auto pSoundDevUUID =
              rMDFactory.dataHolder().musicDeviceId(_soundDevUUID);
          if (!pSoundDevUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          m_eventRoutes.connectedNotes2Notes(*pControllerUUID, widgetIdx, note,
                                            eventIdx, channelIdx,
                                            *pSoundDevUUID, voiceIdx);
       });
   onControllerEventRouterConnectedNotes2Parameter(
       [this, &rMDFactory](
           const ::capnzero::SpanCL<16>& controllerUUID,
           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 note,
           ::capnzero::Int16 eventIdx, ::capnzero::Int16 channelIdx,
           const ::capnzero::SpanCL<16>& soundDevUUID,
           ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
           ::capnzero::Int16 paramFunc) {
          util::Identifiable::UUID _controllerUUID;
          std::copy(controllerUUID.begin(), controllerUUID.end(),
                    _controllerUUID.begin());
          const auto pControllerUUID =
              rMDFactory.dataHolder().musicDeviceId(_controllerUUID);
          if (!pControllerUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          util::Identifiable::UUID _soundDevUUID;
          std::copy(soundDevUUID.begin(), soundDevUUID.end(),
                    _soundDevUUID.begin());
          const auto pSoundDevUUID =
              rMDFactory.dataHolder().musicDeviceId(_soundDevUUID);
          if (!pSoundDevUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          m_eventRoutes.connectedNotes2Parameter(
              *pControllerUUID, widgetIdx, note, eventIdx, channelIdx,
              *pSoundDevUUID, voiceIdx, parameterIdx,
              static_cast<controller::ParameterDestination>(paramFunc));
       });
   onControllerEventRouterConnectedWidget2Notes(
       [this, &rMDFactory](
           const ::capnzero::SpanCL<16>& controllerUUID,
           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
           ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
           ::capnzero::Int16 channelIdx,
           const ::capnzero::SpanCL<16>& soundDevUUID,
           ::capnzero::Int16 voiceIdx) {
          util::Identifiable::UUID _controllerUUID;
          std::copy(controllerUUID.begin(), controllerUUID.end(),
                    _controllerUUID.begin());
          const auto pControllerUUID =
              rMDFactory.dataHolder().musicDeviceId(_controllerUUID);
          if (!pControllerUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          util::Identifiable::UUID _soundDevUUID;
          std::copy(soundDevUUID.begin(), soundDevUUID.end(),
                    _soundDevUUID.begin());
          const auto pSoundDevUUID =
              rMDFactory.dataHolder().musicDeviceId(_soundDevUUID);
          if (!pSoundDevUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          m_eventRoutes.connectedWidget2Notes(
              *pControllerUUID, widgetIdx, widgetCoordX, widgetCoordY, eventIdx,
              channelIdx, *pSoundDevUUID, voiceIdx);
       });
   onControllerEventRouterConnectedWidget2Parameter(
       [this, &rMDFactory](
           const ::capnzero::SpanCL<16>& controllerUUID,
           ::capnzero::Int16 widgetIdx, ::capnzero::Int16 widgetCoordX,
           ::capnzero::Int16 widgetCoordY, ::capnzero::Int16 eventIdx,
           ::capnzero::Int16 channelIdx,
           const ::capnzero::SpanCL<16>& soundDevUUID,
           ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
           ::capnzero::Int16 paramFunc) {
          util::Identifiable::UUID _controllerUUID;
          std::copy(controllerUUID.begin(), controllerUUID.end(),
                    _controllerUUID.begin());
          const auto pControllerUUID =
              rMDFactory.dataHolder().musicDeviceId(_controllerUUID);
          if (!pControllerUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          util::Identifiable::UUID _soundDevUUID;
          std::copy(soundDevUUID.begin(), soundDevUUID.end(),
                    _soundDevUUID.begin());
          const auto pSoundDevUUID =
              rMDFactory.dataHolder().musicDeviceId(_soundDevUUID);
          if (!pSoundDevUUID)
          {
             spdlog::error("No mdId found for uuid");
             return;
          }
          m_eventRoutes.connectedWidget2Parameter(
              *pControllerUUID, widgetIdx, widgetCoordX, widgetCoordY, eventIdx,
              channelIdx, *pSoundDevUUID, voiceIdx, parameterIdx,
              static_cast<controller::ParameterDestination>(paramFunc));
       });
}