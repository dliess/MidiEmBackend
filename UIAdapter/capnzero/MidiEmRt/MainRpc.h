#ifndef RT_MAIN_RPC_H
#define RT_MAIN_RPC_H

#include "MidiEmRtRpcIf.h"
#include "MidiRouter.h"
#include "MusicDeviceContainer.h"
#include "RtServer.h"
#include "TransportControl.h"
#include "AbletonLinkWrapper.h"
#include "ParameterSceneContainer.h"
#include "ControllerEventRouter.h"
#include "Tracks.h"

namespace uiadapter::capnzero
{
class MainRpc : public ::capnzero::MidiEmRt::RpcIf
{
public:
   MainRpc(RtServer::Signals &rSignals,
           base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
           base::TransportControl &rTransportControl,
           base::AbletonLinkWrapper& rAbletonLinkWrapper,
           base::midifriends::Router &rMidiRouter,
           base::eventRouter::EventRouter &rCtrlEventRouter,
           base::musicDevice::sound::ParameterSceneContainer &rParameterSceneContainer,
           base::session::Tracks& rTracks);
   void reEmitSignals() override;

private:
   RtServer::Signals &m_rSignals;
   base::musicDevice::MusicDeviceContainer &m_rMusicDeviceContainer;
   base::TransportControl &m_rTransportControl;
   base::AbletonLinkWrapper& m_rAbletonLinkWrapper;
   base::midifriends::Router &m_rMidiRouter;
   base::eventRouter::EventRouter &m_rCtrlEventRouter;
   base::musicDevice::sound::ParameterSceneContainer &m_rParameterSceneContainer;
   base::session::Tracks& m_rTracks;
};

}   // namespace uiadapter::capnzero

#endif   // RT_MAIN_RPC_H