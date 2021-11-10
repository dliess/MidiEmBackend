#include "RtClient.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

RtClient::RtClient(zmq::context_t& rZmqContext,
                   LoaderServer::Signals& rServerSignals,
                   base::musicDevice::Factory& rMDFactory) :
    ::capnzero::MidiEmRt::MidiEmRtClientSignals(rZmqContext, "tcp://*:5556"),
    m_rServerSignals(rServerSignals),
    m_rMDFactory(rMDFactory)
{
   onSoundDevicesPresetChanged([this](const ::capnzero::TextView& deviceName,
                                      ::capnzero::Int8 engineIdx,
                                      const ::capnzero::TextView& presetName_) {

      const std::string musicDeviceName(deviceName);
      const std::string presetName(presetName_);
      auto& presetCache = m_rMDFactory.dataHolder.presetCache;
      auto it           = presetCache.find(musicDeviceName);
      if (it != presetCache.end())
      {
         const auto attrs =
             it->second->getPresetAttributes(engineIdx, presetName);
         if (attrs)
         {
             m_rServerSignals.Presets__presetAdded(musicDeviceName, engineIdx, presetName, ~attrs->first, ~attrs->second);
         }
         else
         {
            m_rServerSignals.Presets__presetRemoved(musicDeviceName, engineIdx, presetName);
         }
         it->second->save();
      }
   });
}