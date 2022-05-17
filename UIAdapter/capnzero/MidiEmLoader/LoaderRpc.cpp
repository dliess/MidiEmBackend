#include "LoaderRpc.h"

#include "DevicePresets.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base::musicDevice;

LoaderRpc::LoaderRpc(
    LoaderServer::Signals& rSignals,
    base::musicDevice::factory::Factory& rMusicDevicFactory) noexcept :
    m_rSignals(rSignals), m_rMusicDevicFactory(rMusicDevicFactory)
{

   m_rSignals.registerAllEmissionDoneSubscrCb(
       [this](LoaderServer::Signals& signals) {
           reEmitSignals();
       });

   m_rMusicDevicFactory.dataHolder().onDescriptionAdded(
       [this](const std::string& deviceName,
              const description::Description& description) {
          m_rSignals.MusicDevices__musicDeviceDescriptionAdded(
              deviceName, meta::serialize(description).dump().c_str());
       });

   m_rMusicDevicFactory.dataHolder().onPresetUpdated(
       [this](const sound::preset::Id& id, sound::preset::Category category,
              sound::preset::Genre genre) {
          m_rSignals.Presets__presetAdded(id.musicDeviceName, id.engineIdx,
                                          id.presetName, ~category, ~genre);
       });
   m_rMusicDevicFactory.dataHolder().onPresetRemoved(
       [this](const sound::preset::Id& id) {
          m_rSignals.Presets__presetRemoved(id.musicDeviceName, id.engineIdx,
                                            id.presetName);
       });

   m_rMusicDevicFactory.dataHolder().onActualPresetNameChanged(
       [this](const util::Identifiable::UUID& uuid, int voiceIdx,
              const std::string& presetName) {
          m_rSignals.SoundDevices__actualPresetChanged(uuid, voiceIdx,
                                                       presetName);
       });
}

void LoaderRpc::reEmitSignals()
{
   spdlog::info("____ reemitting signals ____");
   m_rMusicDevicFactory.dataHolder().reEmitSignals();
   m_rSignals.allMusicDevicesChanged(m_rMusicDevicFactory.getAllDevicesAsJson());
   m_rSignals.allEmissionDone();
   spdlog::info("_____ allEmissionDone() sent _____");
}

void LoaderRpc::loadMusicDeviceToChain(const ::capnzero::TextView& chainRoot,
                                       const ::capnzero::TextView& manufacturer,
                                       const ::capnzero::TextView& device,
                                       ::capnzero::UInt8 midiVoiceOffset)
{
   std::string mdName(manufacturer);
   mdName.append("/");
   mdName.append(device);
   m_rMusicDevicFactory.loadMusicDeviceToChain(
       base::musicDevice::MusicDeviceId(std::string(chainRoot)), mdName,
       midiVoiceOffset);
}

void LoaderRpc::removeLastMusicDeviceFromChain(
    const ::capnzero::TextView& chainRoot)
{
   m_rMusicDevicFactory.removeLastMusicDeviceFromChain(
       base::musicDevice::MusicDeviceId(std::string(chainRoot)));
}