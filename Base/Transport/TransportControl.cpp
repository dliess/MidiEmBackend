#include "TransportControl.h"
#include "MusicDevice.h"
#include <loguru.hpp>

using namespace base::musicDevice;

TransportControl::TransportControl(
  musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept :
   m_rMusicDeviceContainer(rMusicDeviceContainer)
{
   m_rMusicDeviceContainer.registerForAdd([this](std::shared_ptr<base::musicDevice::MusicDevice> ptr){
      if(ptr->sequencer)
      {
         auto uuid = ptr->id();
         ptr->sequencer->registerTransportMaskChangedCb([this, uuid](bool masked){
            for(auto& cb : m_transportMaskChangedCbs) cb(uuid, masked);
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
   if(it->second->sequencer)
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
   m_started = true;
   for(auto& md : m_rMusicDeviceContainer)
   {
      if(md.second->sequencer)
      {
         md.second->sequencer->start();
      }
   }
   for(auto& cb : m_startedChangeNotifCb) cb(m_started);
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
   for(auto& cb : m_startedChangeNotifCb) cb(m_started);
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

void TransportControl::registerStartedChangeNotifCb(StartedChangeNotifCb cb)
{
   m_startedChangeNotifCb.push_back(cb);
}

void TransportControl::registerTransportMaskChangedCb(TransportMaskChangedCb cb)
{
   m_transportMaskChangedCbs.push_back(cb);
}

void TransportControl::retriggerTransportMaskChangedCbs()
{
   for(auto& md : m_rMusicDeviceContainer)
   {
      if(md.second->sequencer)
      {
         const auto masked = !md.second->sequencer->getEnabled();
         for(auto& cb : m_transportMaskChangedCbs) cb(md.second->id(), masked);
      }
   }
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