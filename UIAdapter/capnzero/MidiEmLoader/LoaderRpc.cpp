#include "LoaderRpc.h"

#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

LoaderRpc::LoaderRpc(
    LoaderServer::Signals& rSignals,
    base::musicDevice::factory::Factory& rMusicDevicFactory) noexcept :
    m_rSignals(rSignals),
    m_rMusicDevicFactory(rMusicDevicFactory)
{
}

void LoaderRpc::reEmitSignals()
{
   m_rMusicDevicFactory.dataHolder().reEmitSignals();
   m_rSignals.allMusicDevicesChanged(m_rMusicDevicFactory.getAllDevicesAsJson());
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