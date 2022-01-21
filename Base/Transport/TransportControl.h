#ifndef TRANSPORT_CONTROL_H
#define TRANSPORT_CONTROL_H

#include <functional>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "Identifiable.h"

namespace base::musicDevice
{
struct Holder;
class TransportControl
{
public:
   TransportControl(
       musicDevice::Holder& rMusicDeviceHolder) noexcept;
   void toggleEnabled(const util::Identifiable::UUID& uuid) noexcept;
   void start() noexcept;
   void stop() noexcept;
   void toggleStartStop() noexcept;
   void retriggerCallbacks();
   void update();

   void setStartOnBeat(bool startOnBeat) noexcept;
   [[nodiscard]] bool getStartOnBeat() const noexcept;

   CB_SIGNAL(StartedChanged, bool);
   CB_SIGNAL(TransportMaskChanged, const util::Identifiable::UUID&, bool);
   CB_SIGNAL(StartOnBeat, bool);

private:
   musicDevice::Holder& m_rMusicDeviceHolder;
   bool m_startOnBeat{true};
   bool m_startRequested{false};
   bool m_started{false};

   void startNow();
};

}   // namespace base::musicDevice
#endif