#include "TransportControl.h"

#include <loguru.hpp>

using namespace base;

TransportControl::TransportControl(
   musicDevice::MidiHolder& rMidiHolder) noexcept :
   m_rMidiHolder(rMidiHolder)
{
   rMidiHolder.registerForOutputAdded(
      [this](
         const std::shared_ptr<musicDevice::MusicDevice::MidiOutput>& pMidiIn) {
         const musicDevice::MidiHolder::Id id(pMidiIn->medium().getDeviceName(),
                                              pMidiIn->medium().getPortName());

         auto it = m_enabledDevices.find(id);
         if (it != m_enabledDevices.end())
         {
            it->second = pMidiIn;
         }
         else
         {
            m_enabledDevices.emplace(std::make_pair(id, pMidiIn));
         }
      }
   );

   rMidiHolder.registerForOutputRemoved(
      [this](const musicDevice::MidiHolder::Id& id) {
         m_enabledDevices.erase(id);
      }
   );
}

void TransportControl::startAllEnabled() noexcept
{
   if (m_started)
      return;
   for (auto& e : m_enabledDevices)
   {
      if (e.second)
      {
         e.second->send(midi::Message<midi::Start>());
      }
   }
   m_started = true;
   if (m_startedChangeNotifCb)
      m_startedChangeNotifCb(m_started);
}

void TransportControl::stopAllEnabled() noexcept
{
   if (!m_started)
      return;
   for (auto& e : m_enabledDevices)
   {
      if (e.second)
      {
         e.second->send(midi::Message<midi::Stop>());
      }
   }
   m_started = false;
   if (m_startedChangeNotifCb)
      m_startedChangeNotifCb(m_started);
}

bool TransportControl::getStarted() const noexcept { return m_started; }

void TransportControl::toggleEnabled(
   const musicDevice::MidiHolder::Id& id) noexcept
{
   auto it = m_enabledDevices.find(id);
   if (it != m_enabledDevices.end())
   {
      if (it->second)
      {
         it->second->send(midi::Message<midi::Stop>());
      }
      m_enabledDevices.erase(id);
   }
   else
   {
      auto pMidiOut = m_rMidiHolder.getMidiOut(id);
      if (pMidiOut && m_started)
      {
         pMidiOut->send(midi::Message<midi::Start>());
      }
      m_enabledDevices.emplace(std::make_pair(id, std::move(pMidiOut)));
   }
   if (m_enableMaskChangeNotifCb)
   {
      m_enableMaskChangeNotifCb(id);
   }
}

bool TransportControl::getIfEnabled(const musicDevice::MidiHolder::Id& id) const
   noexcept
{
   return m_enabledDevices.find(id) != m_enabledDevices.end();
}

void TransportControl::registerStartedChangeNotifCb(
   std::function<void(bool)> cb) noexcept
{
   m_startedChangeNotifCb = cb;
}

void TransportControl::registerEnableMaskChangeNotifCb(
   std::function<void(const musicDevice::MidiHolder::Id&)> cb) noexcept
{
   m_enableMaskChangeNotifCb = cb;
}

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
