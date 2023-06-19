#include "ControllerAdditionalEventCreator.h"

#include "ControllerSection.h"

using namespace base::musicDevice::controller;

AdditionalEventCreator::AdditionalEventCreator(
    const description::controller::Section& rControllerSection) :
    m_rControllerSection(rControllerSection)
{
}

void AdditionalEventCreator::eventReceived(const Event& event)
{
   const auto& eventDescr =
       m_rControllerSection.widgets[event.id.widgetId].events[event.id.eventId];
   SWITCH(eventDescr)
      CASE(description::controller::EventPressRelease, evt) 
      {
      },
      CASE(description::controller::EventContinousValue, evt) 
      {
      },
      CASE(description::controller::EventRelativeValue, evt) 
      {
      },
      CASE(description::controller::EventIncremental, evt) 
      {
      },
      CASE_DEFAULT {}
   END_SWITCH
   SWITCH(event.value)
      CASE(PressReleaseType, value)
      {
         if(value.value < 0)
         {

         }
      },
      CASE(ContinousValueType, value)
      {

      },
      CASE_DEFAULT {}
   END_SWITCH
   emitEventHappened(event);
}