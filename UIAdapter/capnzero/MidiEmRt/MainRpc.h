#ifndef RT_MAIN_RPC_H
#define RT_MAIN_RPC_H

#include "Instruments.h"
#include "MidiEmRtRpcIf.h"
#include "MidiRouter.h"
#include "MusicDeviceContainer.h"
#include "RtServer.h"
#include "TransportControl.h"

namespace uiadapter::capnzero
{
class MainRpc : public ::capnzero::MidiEmRt::RpcIf
{
public:
   MainRpc(RtServer::Signals &rSignals,
           base::instruments::Instruments &rInstruments,
           base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
           base::musicDevice::TransportControl &rTransportControl,
           base::midifriends::Router &rMidiRouter);
   void reEmitSignals() override;

private:
   RtServer::Signals &m_rSignals;
   base::instruments::Instruments &m_rInstruments;
   base::musicDevice::MusicDeviceContainer &m_rMusicDeviceContainer;
   base::musicDevice::TransportControl &m_rTransportControl;
   base::midifriends::Router &m_rMidiRouter;
};

}   // namespace uiadapter::capnzero

#endif   // RT_MAIN_RPC_H