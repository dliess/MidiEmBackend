#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   LoaderServer::Signals& rServerSignals,
                   base::musicDevice::factory::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext, "tcp://*:5556"),
    m_rServerSignals(rServerSignals),
    m_rMDFactory(rMDFactory)
{
   onSoundDevicesPresetChanged([this](const ::capnzero::TextView& deviceName,
                                      ::capnzero::Int8 engineIdx,
                                      const ::capnzero::TextView& presetName) {
      const sound::preset::Id presetId(
          {std::string(deviceName), engineIdx, std::string(presetName)});
      m_rMDFactory.dataHolder().soundDevicesPresetChanged(presetId);
   });

   m_rMDFactory.dataHolder().onDescriptionAdded(
       [this](const std::string& deviceName,
              const description::Description& description) {
          m_rServerSignals.MusicDevices__musicDeviceDescriptionAdded(
              deviceName, meta::serialize(description).dump().c_str());
       });

   m_rMDFactory.dataHolder().onPresetUpdated(
       [this](const sound::preset::Id& id, sound::preset::Category category,
              sound::preset::Genre genre) {
          m_rServerSignals.Presets__presetAdded(id.musicDeviceName,
                                                id.engineIdx, id.presetName,
                                                ~category, ~genre);
       });
   m_rMDFactory.dataHolder().onPresetRemoved(
       [this](const sound::preset::Id& id) {
          m_rServerSignals.Presets__presetRemoved(id.musicDeviceName,
                                                  id.engineIdx, id.presetName);
       });
}