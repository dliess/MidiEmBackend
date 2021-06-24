#include "LoaderRpc.h"
#include "MusicDeviceFactory.h"

using namespace uiadapter::capnzero;

LoaderRpc::LoaderRpc(base::musicDevice::Factory& rMusicDevicFactory) noexcept :
    m_rMusicDevicFactory(rMusicDevicFactory)
{
}

void LoaderRpc::loadMusicDeviceToChain(const ::capnzero::TextView& chainRoot,
                                       const ::capnzero::TextView& manufacturer,
                                       const ::capnzero::TextView& device,
                                       ::capnzero::UInt8 midiVoiceOffset)
{
    std::string mdName(manufacturer);
    mdName.append("/");
    mdName.append(device);
    m_rMusicDevicFactory.loadMusicDeviceToChain(base::musicDevice::MusicDeviceId(std::string(chainRoot)), mdName, midiVoiceOffset);
}

void LoaderRpc::removeLastMusicDeviceFromChain(
    const ::capnzero::TextView& chainRoot)
{
    m_rMusicDevicFactory.removeLastMusicDeviceFromChain(base::musicDevice::MusicDeviceId(std::string(chainRoot)));
}