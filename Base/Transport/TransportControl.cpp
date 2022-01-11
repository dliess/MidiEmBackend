#include "TransportControl.h"
#include "MusicDeviceContainer.h"
#include <loguru.hpp>

using namespace base::musicDevice;

TransportControl::TransportControl(
  musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
   m_rMusicDeviceContainer(rMusicDeviceContainer)
{
   m_rMusicDeviceContainer.onAdded([this](std::shared_ptr<base::musicDevice::MusicDevice> ptr){
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
   auto it = m_rMusicDeviceContainer.find(uuid);
   if(it == m_rMusicDeviceContainer.end())
   {
      //LOG_F(ERROR, "UUID {} should be found in MusicDevices", uuid);
      return;
   }
   if(!it->second->sequencer)
   {
      //LOG_F(ERROR, "UUID {} in MusicDevices has no sequencer", uuid);
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
   for(auto& md : m_rMusicDeviceContainer)
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
   for(auto& md : m_rMusicDeviceContainer)
   {
      if(md.second->sequencer)
      {
         const auto masked = !md.second->sequencer->getEnabled();
         emitTransportMaskChanged(md.second->id(), masked);
      }
   }
   emitStartedChanged(m_started);
}

void TransportControl::update()
{
   if(!m_startRequested)
   {
      return;
   }
   if(m_startOnBeat)
   {
      static constexpr int threshold = tempo::BeatTick::PPQ / 10;
      const int rest = tempo::BeatTick::instance().getBeatJiffies() % tempo::BeatTick::PPQ;
      if(rest < threshold)
      {
         //TODO: compensate
         startNow();
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
   for(auto& md : m_rMusicDeviceContainer)
   {
      if(md.second->sequencer)
      {
         md.second->sequencer->start();
      }
   }
   emitStartedChanged(m_started);
}


/*

TransportControl::Settings TransportControl::getSettings() const noexcept
{
   Settings settings;
   for (auto& e : m_enabledDevices)
   {
      if (e.second)
      {
         settings.push_back(e.first.toStr());
      }
   }
   return settings;
}

void TransportControl::setSettings(const Settings& settings) noexcept
{
   for (auto& e : settings)
   {
      const musicDevice::MidiHolder::Id id(e);
      auto pMidiOut = m_rMidiHolder.getMidiOut(id);
      m_enabledDevices.emplace(std::make_pair(id, std::move(pMidiOut)));
   }
}

*/