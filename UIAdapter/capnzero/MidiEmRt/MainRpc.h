#ifndef RT_MAIN_RPC_H
#define RT_MAIN_RPC_H

#include "Instruments.h"
#include "MidiEmRtRpcIf.h"
#include "MidiRouter.h"
#include "MusicDeviceContainer.h"
#include "RtServer.h"
#include "TransportControl.h"
#include "AbletonLinkWrapper.h"
#include "ParameterSceneContainer.h"
#include "ControllerEventRouter.h"

namespace uiadapter::capnzero
{
class MainRpc : public ::capnzero::MidiEmRt::RpcIf
{
public:
   MainRpc(RtServer::Signals &rSignals,
           base::instruments::Instruments &rInstruments,
           base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
           base::TransportControl &rTransportControl,
           base::AbletonLinkWrapper& rAbletonLinkWrapper,
           base::midifriends::Router &rMidiRouter,
           base::musicDevice::controller::EventRouter &rCtrlEventRouter,
           base::musicDevice::sound::ParameterSceneContainer &rParameterSceneContainer);
   void reEmitSignals() override;

private:
   RtServer::Signals &m_rSignals;
   base::instruments::Instruments &m_rInstruments;
   base::musicDevice::MusicDeviceContainer &m_rMusicDeviceContainer;
   base::TransportControl &m_rTransportControl;
   base::AbletonLinkWrapper& m_rAbletonLinkWrapper;
   base::midifriends::Router &m_rMidiRouter;
   base::musicDevice::controller::EventRouter &m_rCtrlEventRouter;
   base::musicDevice::sound::ParameterSceneContainer &m_rParameterSceneContainer;
};

}   // namespace uiadapter::capnzero

#endif   // RT_MAIN_RPC_H