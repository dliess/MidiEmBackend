#include "AbletonLinkWrapper.h"

#include <ableton/Link.hpp>
#include <loguru.hpp>

#include "BeatTick.h"
#include "TransportControl.h"

base::AbletonLinkWrapper::AbletonLinkWrapper(base::musicDevice::TransportControl& rTransportControl) :
    m_pAbletonLink(std::make_unique<ableton::Link>(120),
    m_rTransportControl(rTransportControl)
{
   m_pAbletonLink->setTempoCallback([](double tempo) {
      LOG_F(INFO, "Ableton-Link :: Tempo changed: {}", tempo);
   });
   m_pAbletonLink->setStartStopCallback([this](bool start) {
      LOG_F(INFO, "Ableton-Link :: StartStop changed: {}", start);
      if (m_reactsOnTransport)
      {
         emitStartStopChanged(start);
      }
   });
   m_pAbletonLink->setNumPeersCallback([this](size_t numPeers) {
      LOG_F(INFO, "Ableton-Link :: NumPeersChanged: {}", numPeers);
      emitNumPeersChanged(numPeers);
   });
}

// Dummy for unique_ptr forward decl
base::AbletonLinkWrapper::~AbletonLinkWrapper() = default;

void base::AbletonLinkWrapper::enable(bool enable)
{
   if (m_pAbletonLink->isEnabled() != enable)
   {
      m_pAbletonLink->enable(enable);
      emitEnabledChanged(enable);
   }
}

bool base::AbletonLinkWrapper::isEnabled() const
{
   return m_pAbletonLink->isEnabled();
}

void base::AbletonLinkWrapper::reactOnTransport(bool react) noexcept
{
   if (m_reactsOnTransport != react)
   {
      m_reactsOnTransport = react;
      emitReactsOnTransportChanged(m_reactsOnTransport);
   }
}

bool base::AbletonLinkWrapper::reactsOnTransport() const noexcept
{
   return m_reactsOnTransport;
}

void base::AbletonLinkWrapper::setTempo(double bpm)
{
   auto session = m_pAbletonLink->captureAudioSessionState();
   session.setTempo(bpm, m_pAbletonLink->clock().micros());
   m_pAbletonLink->commitAudioSessionState(session);
}

std::pair<double, double> base::AbletonLinkWrapper::snapshot()
{
   auto session = m_pAbletonLink->captureAudioSessionState();
   return std::make_pair(
       session.tempo(),
       session.beatAtTime(m_pAbletonLink->clock().micros(), 4));
}

void base::AbletonLinkWrapper::retriggerCallbacks()
{
   emitEnabledChanged(m_pAbletonLink->isEnabled());
   emitReactsOnTransportChanged(m_reactsOnTransport);
   emitNumPeersChanged(m_pAbletonLink->numPeers());
}
