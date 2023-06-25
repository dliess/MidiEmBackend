#ifndef BASE_MUSIC_DEVICE_CONTROLLER_ADDITIONAL_EVENT_CREATOR_H
#define BASE_MUSIC_DEVICE_CONTROLLER_ADDITIONAL_EVENT_CREATOR_H

#include "CallbackSignal.h"
#include "ControllerEvents.h"
#include <list>

// clang-format off
namespace base::musicDevice::description::controller { struct Section; }
// clang-format on

namespace base::musicDevice::controller
{
class AdditionalEventCreator
{
public:
   explicit AdditionalEventCreator(
       const description::controller::Section& rControllerSection);
    void eventReceived(const Event& event);
   CB_SIGNAL(EventHappened, const Event&);
private:
   const description::controller::Section& m_rControllerSection;
   std::list<Event> m_ongoingContinousEventStartPoints;
   std::list<Event> m_lastContOrRelEventValues;
   std::list<EventId> m_independentPressList;
   constexpr static float VALUE_JUMP_THRESHOLD = 0.1f;
   constexpr static int DERIVED_INCREMENT_RESOLUTION = 128;

   void createExtraEvents4PressReleaseEvent(const description::controller::EventPressRelease& evtDescr,
                                           const Event& event);
   void createExtraEvents4ContinousEvent(const std::vector<description::controller::Event>& eventsDescr,
                                         const Event& event, int sourceEvtIdx);
   void createExtraEvents4RelativeEvent(const std::vector<description::controller::Event>& eventsDescr,
                                        const Event& event, int sourceEvtIdx);
   void createExtraEvents4IncrementalEvent(const std::vector<description::controller::Event>& eventsDescr,
                                           const Event& event, int sourceEvtIdx);
   void createFromContinousToRelative(const Event& event, int destEvtIdx);
   void createFromContinousToIncremental(const Event& event, int destEvtIdx);
   void createFromRelativeToIncremental(const Event& event, int destEvtIdx);
   void createFromIncrementalToRelative(const Event& event, int destEvtIdx);
};

}   // namespace base::musicDevice::controller
#endif