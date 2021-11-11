#ifndef MIDIEM_LOADER_RPC_H
#define MIDIEM_LOADER_RPC_H

#include "MidiEmLoaderRpcIf.h"

namespace base::musicDevice::factory
{
class Factory;
}

namespace uiadapter::capnzero
{
class LoaderRpc : public ::capnzero::MidiEmLoader::RpcIf
{
public:
   LoaderRpc(base::musicDevice::factory::Factory& rMusicDevicFactory) noexcept;
   void loadMusicDeviceToChain(const ::capnzero::TextView& chainRoot,
                               const ::capnzero::TextView& manufacturer,
                               const ::capnzero::TextView& device,
                               ::capnzero::UInt8 midiVoiceOffset) override;
   void removeLastMusicDeviceFromChain(
       const ::capnzero::TextView& chainRoot) override;

private:
   base::musicDevice::factory::Factory& m_rMusicDevicFactory;
};

}   // namespace uiadapter::capnzero

#endif   // MIDIEM_LOADER_RPC_H