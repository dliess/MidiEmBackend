#include "RtClient.h"

#include "ControllerEventRouterLoader.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(
    zmq::context_t& rZmqContext, const std::string& rtServerRpcAddr,
    const std::string& rtServerSignalAddr, LoaderServer::Signals& rSignals,
    base::musicDevice::factory::Factory& rMDFactory,
    base::musicDevice::controller::loader::EventRoutes& rEventRoutes) :
    ::capnzero::MidiEmRt::MidiEmRtClientRpc(rZmqContext, rtServerRpcAddr),
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext,
                                                rtServerSignalAddr),
    m_rEventRoutes(rEventRoutes)
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
       [&rMDFactory, &rSignals, this](const ::capnzero::SpanCL<16>& uuid,
                                      const ::capnzero::TextView& type,
                                      const ::capnzero::TextView& port,
                                      const ::capnzero::TextView& mediumId,
                                      ::capnzero::Int16 midiVoiceOffset) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          const MusicDeviceId mdId{std::string(type), std::string(port)};
          rMDFactory.dataHolder().addUuid2MdId(uuidData, mdId);
          m_rEventRoutes.musicDeviceAppeared(mdId);
       });
   onMusicDevicesDeviceRemoved(
       [&rMDFactory, &rSignals, this](const ::capnzero::SpanCL<16>& uuid) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().removeEntryForUuid(uuidData);
          const auto pMdId = rMDFactory.dataHolder().musicDeviceId(uuidData);
          if(pMdId)
          {
            m_rEventRoutes.musicDeviceDisappeared(*pMdId);
          }
       });

   onAbletonLinkEnabledChanged([](::capnzero::Bool val) {
      // TODO: save it to file and load
   });
   onAbletonLinkOffsetTimeUsChanged([](::capnzero::Int32 val) {
      // TODO: save it to file and load
   });

   m_rEventRoutes.onConnectionLoadedNotes2Notes(
       [this, &rMDFactory](const MusicDeviceId& controllerID, int widgetIdx,
                           int note, int eventIdx, int channelIdx,
                           const MusicDeviceId& soundDevID, int voiceIdx) {
          const auto pControllerUUID =
              rMDFactory.dataHolder().getUUIDByMdId(controllerID);
          const auto psoundDevUUID =
              rMDFactory.dataHolder().getUUIDByMdId(soundDevID);
          if (pControllerUUID != nullptr && psoundDevUUID != nullptr)
          {
             ControllerEventRouter__connectNotes2Notes(
                 *pControllerUUID, widgetIdx, note, eventIdx, channelIdx,
                 *psoundDevUUID, voiceIdx);
          }
       });
   m_rEventRoutes.onConnectionLoadedNotes2Parameter(
       [this, &rMDFactory](const MusicDeviceId& controllerID, int widgetIdx,
                           int note, int eventIdx, int channelIdx,
                           const MusicDeviceId& soundDevID, int voiceIdx,
                           int parameterIdx,
                           controller::ParameterDestination paramFunc) {
          const auto pControllerUUID =
              rMDFactory.dataHolder().getUUIDByMdId(controllerID);
          const auto psoundDevUUID =
              rMDFactory.dataHolder().getUUIDByMdId(soundDevID);
          if (pControllerUUID != nullptr && psoundDevUUID != nullptr)
          {
             ControllerEventRouter__connectNotes2Parameter(
                 *pControllerUUID, widgetIdx, note, eventIdx, channelIdx,
                 *psoundDevUUID, voiceIdx, parameterIdx,
                 static_cast<::capnzero::MidiEmRt::SDParameterDestination>(paramFunc));
          }
       });
   m_rEventRoutes.onConnectionLoadedWidget2Notes(
       [this, &rMDFactory](const MusicDeviceId& controllerID, int widgetIdx,
                           int widgetCoordX, int widgetCoordY, int eventIdx,
                           int channelIdx, const MusicDeviceId& soundDevID,
                           int voiceIdx) {
          const auto pControllerUUID =
              rMDFactory.dataHolder().getUUIDByMdId(controllerID);
          const auto psoundDevUUID =
              rMDFactory.dataHolder().getUUIDByMdId(soundDevID);
          if (pControllerUUID != nullptr && psoundDevUUID != nullptr)
          {
             ControllerEventRouter__connectWidget2Notes(
                 *pControllerUUID, widgetIdx, widgetCoordX, widgetCoordY,
                 eventIdx, channelIdx, *psoundDevUUID, voiceIdx);
          }
       });
   m_rEventRoutes.onConnectionLoadedWidget2Parameter(
       [this, &rMDFactory](const MusicDeviceId& controllerID, int widgetIdx,
                           int widgetCoordX, int widgetCoordY, int eventIdx,
                           int channelIdx, const MusicDeviceId& soundDevID,
                           int voiceIdx, int parameterIdx,
                           controller::ParameterDestination paramFunc) {
          const auto pControllerUUID =
              rMDFactory.dataHolder().getUUIDByMdId(controllerID);
          const auto psoundDevUUID =
              rMDFactory.dataHolder().getUUIDByMdId(soundDevID);
          if (pControllerUUID != nullptr && psoundDevUUID != nullptr)
          {
             ControllerEventRouter__connectWidget2Parameter(
                 *pControllerUUID, widgetIdx, widgetCoordX, widgetCoordY,
                 eventIdx, channelIdx, *psoundDevUUID, voiceIdx, parameterIdx,
                 static_cast<::capnzero::MidiEmRt::SDParameterDestination>(paramFunc));
          }
       });
}