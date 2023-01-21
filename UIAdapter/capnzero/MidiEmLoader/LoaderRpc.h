#ifndef MIDIEM_LOADER_RPC_H
#define MIDIEM_LOADER_RPC_H

#include "MidiEmLoaderRpcIf.h"
#include "LoaderServer.h"


// clang-format off
namespace base::musicDevice::factory { class Factory; }
namespace base::instruments { class Instruments; }
// clang-format on
namespace uiadapter::capnzero
{
class LoaderRpc : public ::capnzero::MidiEmLoader::RpcIf
{
public:
   LoaderRpc(LoaderServer::Signals& rSignals,
             base::instruments::Instruments &rInstruments,
             base::musicDevice::factory::Factory& rMusicDevicFactory) noexcept;
   void reEmitSignals() override;
   void loadMusicDeviceToChain(const ::capnzero::TextView& chainRoot,
                               const ::capnzero::TextView& manufacturer,
                               const ::capnzero::TextView& device,
                               ::capnzero::Int16 midiVoiceOffset) override;
   void removeLastMusicDeviceFromChain(
       const ::capnzero::TextView& chainRoot) override;

private:
   LoaderServer::Signals& m_rSignals;
   base::instruments::Instruments &m_rInstruments;
   base::musicDevice::factory::Factory& m_rMusicDevicFactory;
};

}   // namespace uiadapter::capnzero

#endif   // MIDIEM_LOADER_RPC_H