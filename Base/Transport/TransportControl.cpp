#include "TransportControl.h"
#include "MusicDeviceHolder.h"
#include <spdlog/spdlog.h>

using namespace base::musicDevice;

TransportControl::TransportControl(
  musicDevice::Holder& rMusicDeviceHolder) noexcept :
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
   m_rMusicDeviceHolder.musicDevices.onAdded([this](std::shared_ptr<base::musicDevice::MusicDevice> ptr){
      if(ptr->sequencer)
      {
         auto uuid = ptr->id();
         ptr->sequencer->registerTransportMaskChangedCb([this, uuid](bool masked){
            emitTransportMaskChanged(uuid, masked);
         });
      }
   });
}

void TransportControl::toggleEnabled(const util::Identifiable::UUID& uuid) noexcept
{
   auto it = m_rMusicDeviceHolder.musicDevices.find(uuid);
   if(it == m_rMusicDeviceHolder.musicDevices.end())
   {
      //spdlog::error( "UUID {} should be found in MusicDevices", uuid);
      return;
   }
   if(!it->second->sequencer)
   {
      //spdlog::error( "UUID {} in MusicDevices has no sequencer", uuid);
      return;
   }
   it->second->sequencer->toggleEnabled();
   if(m_started) it->second->sequencer->start();
}

void TransportControl::start() noexcept
{
   if(m_started) return;
   m_startRequested = true;
}

void TransportControl::stop() noexcept
{
   if(!m_started) return;
   m_started = false;
   for(auto& md : m_rMusicDeviceHolder.musicDevices)
   {
      if(md.second->sequencer)
      {
         md.second->sequencer->stop();
      }
   }
   emitStartedChanged(m_started);
}

void TransportControl::toggleStartStop() noexcept
{
   if(m_started)
   {
      stop();
   }
   else
   {
      start();
   }
}

void TransportControl::retriggerCallbacks()
{
   for(auto& md : m_rMusicDeviceHolder.musicDevices)
   {
      if(md.second->sequencer)
      {
         const auto masked = !md.second->sequencer->getEnabled();
         emitTransportMaskChanged(md.second->id(), masked);
      }
   }
   emitStartedChanged(m_started);
   emitStartOnBeat(m_startOnBeat);
}

void TransportControl::update()
{
   if(!m_startRequested)
   {
      return;
   }
   if(m_startOnBeat)
   {
      static constexpr double Threshold = 0.1;
      const auto beat = tempo::BeatTick::instance().getBeat();
      const double rest = beat - static_cast<int>(beat);
      if(rest < Threshold)
      {
         startNow();
         m_rMusicDeviceHolder.midiHolder.midiClock(rest); // fast forward devices midi-time
      }
   }
   else
   {
      startNow();
   }
}

void TransportControl::startNow()
{
   m_started = true;
   m_startRequested = false;
   for(auto& md : m_rMusicDeviceHolder.musicDevices)
   {
      if(md.second->sequencer)
      {
         md.second->sequencer->start();
      }
   }
   emitStartedChanged(m_started);
}

void TransportControl::setStartOnBeat(bool startOnBeat) noexcept
{ 
   if(m_startOnBeat != startOnBeat)
   {
      m_startOnBeat = startOnBeat;
      emitStartOnBeat(m_startOnBeat);
   }
};

bool TransportControl::getStartOnBeat() const noexcept
{
   return m_startOnBeat;
};