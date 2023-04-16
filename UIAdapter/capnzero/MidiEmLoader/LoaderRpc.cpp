#include "LoaderRpc.h"

#include "DevicePresets.h"
#include "Instruments.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;
using namespace base;
using namespace base::musicDevice;

LoaderRpc::LoaderRpc(LoaderServer::Signals& rSignals,
                     base::instruments::Instruments& rInstruments,
                     base::musicDevice::factory::Factory& rMusicDevicFactory,
                     base::eventRouter::EventRouter& rCtrlEventRouter) noexcept
    :
    m_rSignals(rSignals),
    m_rInstruments(rInstruments),
    m_rMusicDevicFactory(rMusicDevicFactory),
    m_rCtrlEventRouter(rCtrlEventRouter)
{
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
   m_rInstruments.onDataChanged(
       [this](const instruments::Data& data, bool doSaveToFile) {
          m_rSignals.Instruments__kitInstrumentsChanged(
              meta::serialize(data.kitInstruments).dump().c_str());
          m_rSignals.Instruments__melodicInstrumentsChanged(
              meta::serialize(data.melodicInstruments).dump().c_str());
       });
}

void LoaderRpc::reEmitSignals()
{
   m_rMusicDevicFactory.dataHolder().reEmitSignals();
   m_rSignals.allMusicDevicesChanged(
       m_rMusicDevicFactory.getAllDevicesAsJson());
   m_rInstruments.reEmitSignals();
   m_rCtrlEventRouter.retriggerCallbacks();
}

void LoaderRpc::loadMusicDeviceToChain(const ::capnzero::TextView& chainRoot,
                                       const ::capnzero::TextView& manufacturer,
                                       const ::capnzero::TextView& device,
                                       ::capnzero::Int16 midiVoiceOffset)
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