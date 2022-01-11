#include "AbletonLinkWrapper.h"

#include <ableton/Link.hpp>
#include <loguru.hpp>

#include "BeatTick.h"
#include "TransportControl.h"

base::AbletonLinkWrapper::AbletonLinkWrapper(
    musicDevice::TransportControl& rTransportControl) :
    m_rTransportControl(rTransportControl),
    m_pAbletonLink(std::make_unique<ableton::Link>(
        base::tempo::BeatTick::instance().getBpmCentsNudged() / 100.0))
{
   m_pAbletonLink->setTempoCallback([](double tempo) {
      LOG_F(INFO, "Ableton-Link :: Tempo changed: {}", tempo);
   });
   m_pAbletonLink->setStartStopCallback([](bool start) {
      LOG_F(INFO, "Ableton-Link :: StartStop changed: {}", start);
   });
   m_pAbletonLink->setNumPeersCallback([](size_t numPeers) {
      LOG_F(INFO, "Ableton-Link :: NumPeersChanged: {}", numPeers);
   });
   m_pAbletonLink->enable(true);

   base::tempo::BeatTick::instance().onBpmNudgedChanged([this](int bpmCents) {
      if (m_pAbletonLink->isEnabled())
      {
         auto session = m_pAbletonLink->captureAudioSessionState();
         session.setTempo(bpmCents / 100.0, m_pAbletonLink->clock().micros());
         m_pAbletonLink->commitAudioSessionState(session);
      }
   });
}

// Dummy for unique_ptr forward decl
base::AbletonLinkWrapper::~AbletonLinkWrapper() = default;

void base::AbletonLinkWrapper::enable(bool enable)
{
   m_pAbletonLink->enable(enable);
}

bool base::AbletonLinkWrapper::isEnabled() const
{
   return m_pAbletonLink->isEnabled();
}

void base::AbletonLinkWrapper::update()
{
   auto session = m_pAbletonLink->captureAudioSessionState();
   tempo::BeatTick::instance().setBpmCentsNudged(session.tempo() * 100);
   tempo::BeatTick::instance().setBeatJiffies(
       tempo::BeatTick::PPQ *
       session.beatAtTime(m_pAbletonLink->clock().micros(), 4));
}