#include "RtClient.h"

#include "Instruments.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   const std::string& rtServerRpcAddr,
                   const std::string& rtServerSignalAddr,
                   LoaderServer::Signals& rSignals,
                   base::musicDevice::factory::Factory& rMDFactory,
                   base::instruments::Instruments& rInstruments) :
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
       [&rMDFactory, &rSignals, this](const ::capnzero::SpanCL<16>& uuid,
                                      const ::capnzero::TextView& type,
                                      const ::capnzero::TextView& port,
                                      const ::capnzero::TextView& mediumId,
                                      ::capnzero::Int16 midiVoiceOffset) {
          /*
           ::capnzero::Data<16> uuidData;
           std::copy(uuid.begin(), uuid.end(), uuidData.begin());
           const MusicDeviceId mdId{std::string(type), std::string(port)};
           //rMDFactory.dataHolder().addUuid2MdId(uuidData, mdId);
           // TODO m_rEventRoutes.musicDeviceAppeared(mdId);
           */
       });
   onMusicDevicesDeviceRemoved(
       [&rMDFactory, &rSignals, this](const ::capnzero::SpanCL<16>& uuid) {
          ::capnzero::Data<16> uuidData;
          std::copy(uuid.begin(), uuid.end(), uuidData.begin());
          rMDFactory.dataHolder().removeEntryForUuid(uuidData);
          const auto pMdId = rMDFactory.dataHolder().getMdIdByUUID(uuidData);
          if (pMdId)
          {
             // TODO m_rEventRoutes.musicDeviceDisappeared(*pMdId);
          }
       });

   onAbletonLinkEnabledChanged([](::capnzero::Bool val) {
      // TODO: save it to file and load
   });
   onAbletonLinkOffsetTimeUsChanged([](::capnzero::Int32 val) {
      // TODO: save it to file and load
   });
   onInstrumentsPlayKitParamChanged(
       [&rInstruments](
           const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
           ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
           ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
           ::capnzero::Float32 value) {
          util::Identifiable::UUID uuidCopy;
          std::ranges::copy(uuid, uuidCopy.begin());
          try
          {
             rInstruments.kitParamChanged(
                 uuidCopy, voiceIdx, componentIdx, parameterIdx,
                 static_cast<base::musicDevice::sound::ParameterAttr>(
                     parameterAttr),
                 value);
          }
          catch (const std::exception& e)
          {
             spdlog::error(
                 "Exception occured in onInstrumentsPlayKitParamChanged: {}",
                 e.what());
          }
       });
   onInstrumentsPlayMelodicParamChanged([&rInstruments](
                                            const ::capnzero::SpanCL<16>& uuid,
                                            ::capnzero::Int16 componentIdx,
                                            ::capnzero::Int16 parameterIdx,
                                            ::capnzero::MidiEmRt::
                                                SDParameterAttr parameterAttr,
                                            ::capnzero::Float32 value) {
      util::Identifiable::UUID uuidCopy;
      std::ranges::copy(uuid, uuidCopy.begin());
      try
      {
         rInstruments.melodicParamChanged(
             uuidCopy, componentIdx, parameterIdx,
             static_cast<base::musicDevice::sound::ParameterAttr>(
                 parameterAttr),
             value);
      }
      catch (const std::exception& e)
      {
         spdlog::error(
             "Exception occured in onInstrumentsPlayMelodicParamChanged: {}",
             e.what());
      }
   });
}