#ifndef TRANSPORT_CONTROL_H
#define TRANSPORT_CONTROL_H

#include <functional>
#include <string>
#include <vector>

#include "Identifiable.h"

namespace base::musicDevice
{
class MusicDeviceContainer;
class TransportControl   //: public utils::Settings<TransportControl>
{
public:
   TransportControl(
       musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept;
   void toggleEnabled(const util::Identifiable::UUID& uuid) noexcept;
   void start() noexcept;
   void stop() noexcept;
   void toggleStartStop() noexcept;
   using StartedChangeNotifCb = std::function<void(bool)>;
   void registerStartedChangeNotifCb(StartedChangeNotifCb cb);
   using TransportMaskChangedCb = std::function<void(const util::Identifiable::UUID&, bool)>;
   void registerTransportMaskChangedCb(TransportMaskChangedCb cb);
   void retriggerCallbacks();
   void update();

   /*
      // ============== Settings ===============
      using Settings = std::vector<std::string>;
      Settings getSettings() const noexcept;
      void setSettings(const Settings& settings) noexcept;
      // =======================================
   */
private:
   musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
   bool m_startRequested{false};
   bool m_started{false};
   std::vector<StartedChangeNotifCb> m_startedChangeNotifCb;
   std::vector<TransportMaskChangedCb> m_transportMaskChangedCbs;
};

}   // namespace base::musicDevice
#endif