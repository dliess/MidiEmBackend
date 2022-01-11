#ifndef TRANSPORT_CONTROL_H
#define TRANSPORT_CONTROL_H

#include <functional>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "Identifiable.h"

namespace base::musicDevice
{
class MusicDeviceContainer;
class TransportControl
{
public:
   TransportControl(
       musicDevice::MusicDeviceContainer& rMusicDeviceContainer) noexcept;
   void toggleEnabled(const util::Identifiable::UUID& uuid) noexcept;
   void start() noexcept;
   void stop() noexcept;
   void toggleStartStop() noexcept;
   void retriggerCallbacks();
   void update();

   void setStartOnBeat(bool startOnBeat) noexcept { m_startOnBeat = startOnBeat; };

   CB_SIGNAL(StartedChanged, bool);
   CB_SIGNAL(TransportMaskChanged, const util::Identifiable::UUID&, bool);

private:
   musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
   bool m_startOnBeat{false};
   bool m_startRequested{false};
   bool m_started{false};

   void startNow();
};

}   // namespace base::musicDevice
#endif