#ifndef TRANSPORT_CONTROL_H
#define TRANSPORT_CONTROL_H

#include <functional>
#include <string>
#include <vector>

#include "MusicDevice.h"
#include "MusicDeviceHolder.h"
#include "Settings.h"

namespace base
{
class TransportControl : public utils::Settings<TransportControl>
{
public:
   TransportControl(musicDevice::MidiHolder& rMidiHolder) noexcept;
   void startAllEnabled() noexcept;
   void stopAllEnabled() noexcept;
   bool getStarted() const noexcept;
   void toggleEnabled(const musicDevice::MidiHolder::Id& id) noexcept;
   bool getIfEnabled(const musicDevice::MidiHolder::Id& id) const noexcept;
   void registerStartedChangeNotifCb(std::function<void(bool)> cb) noexcept;
   void registerEnableMaskChangeNotifCb(
      std::function<void(const musicDevice::MidiHolder::Id&)> cb) noexcept;
   // ============== Settings ===============
   using Settings = std::vector<std::string>;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
private:
   musicDevice::MidiHolder& m_rMidiHolder;
   std::unordered_map<musicDevice::MidiHolder::Id,
                      std::shared_ptr<musicDevice::MusicDevice::MidiOutput>>
      m_enabledDevices;
   bool m_started{false};
   std::function<void(bool)> m_startedChangeNotifCb;
   std::function<void(const musicDevice::MidiHolder::Id&)>
      m_enableMaskChangeNotifCb;
};

} // namespace base
#endif