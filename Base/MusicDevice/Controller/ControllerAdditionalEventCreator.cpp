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
   const auto& eventsDescr = m_rControllerSection.widgets[event.id.widgetId].events;
   const auto& eventDescr = eventsDescr[event.id.eventId];

   SWITCH(eventDescr)
      CASE(description::controller::EventPressRelease, evt)
      {
         const auto value = mpark::get<PressReleaseType>(event.value).value;
         if((value > 0.0))
         {
            if(evt.pressVelocityEvtIdx)
            {
               EventId derivedEvtId(event.id);
               derivedEvtId.eventId = evt.pressVelocityEvtIdx.value();
               emitEventHappened(Event{derivedEvtId, ContinousValueType{value}});
            }
            if(evt.independent.value_or(false))
            {
               m_independentPressList.push_back(event.id);
            }
         }
         if((value <= 0.0))
         {
            if(evt.releaseVelocityEvtIdx)
            {
               EventId derivedEvtId(event.id);
               derivedEvtId.eventId = evt.releaseVelocityEvtIdx.value();
               emitEventHappened(Event{derivedEvtId, ContinousValueType{value}});
            }
            if(evt.independent.value_or(false))
            {
               auto it = std::ranges::find(m_independentPressList, event.id);
               if(it != m_independentPressList.end())
               {
                  m_independentPressList.erase(it);
               }
            }
         }
      },
      CASE(description::controller::EventContinousValue, evt)
      {
         if(evt.twin)
         {
            EventId indepPREvtId(event.id);
            indepPREvtId.eventId = evt.twin->indepPressEvtIdx;
            const int evtIdx =
            std::ranges::find(m_independentPressList, indepPREvtId) != 
               m_independentPressList.end() ? evt.twin->twinEvtIdx : event.id.eventId;
            {

            }
         }
      },
      CASE(description::controller::EventRelativeValue, evt)
      {

      },
      CASE(description::controller::EventIncremental, evt)
      {

      },
      CASE_DEFAULT {}
   END_SWITCH



   const auto pressRelease = mpark::get_if<PressReleaseType>(&event.value);
   if(pressRelease && (pressRelease->value > 0.0))
   {
      m_ongoingContinousEventStartPoints.remove_if([&event](const Event& e){
         return e.id.widgetId == event.id.widgetId &&
                e.id.widgetCoord == event.id.widgetCoord;
      });
   }
   for(int eventIdx = int(eventsDescr.size()) - 1; 0 <= eventIdx; --eventIdx)
   {
      EventId derivedEvtId(event.id);
      derivedEvtId.eventId = eventIdx;

      SWITCH(eventsDescr[eventIdx])
         CASE(description::controller::EventDerivedContinousValue, derivedEvt)
         {
            if(derivedEvt.sourceEventIdx == event.id.eventId)
            {
               SWITCH(eventDescr)
                  CASE(description::controller::EventPressRelease, evt) 
                  {
                     const auto pressVel = mpark::get<PressReleaseType>(event.value).value;
                     if((derivedEvt.name == "PressVelocity") && (pressVel > 0.0))
                     {
                        emitEventHappened(Event{derivedEvtId, ContinousValueType{pressVel}});
                     }
                     else if((derivedEvt.name == "ReleaseVelocity") && (pressVel <= 0.0))
                     {
                        emitEventHappened(Event{derivedEvtId, ContinousValueType{pressVel}});
                     }
                  },
                  CASE_DEFAULT {}
               END_SWITCH
            }
         },
         CASE(description::controller::EventDerivedRelativeValue, derivedEvt)
         {
            if(derivedEvt.sourceEventIdx == event.id.eventId)
            {
               auto it = std::ranges::find_if(m_ongoingContinousEventStartPoints, [&event](const Event& e){
                  return e.id == event.id;
               });
               SWITCH(eventDescr)
                  CASE(description::controller::EventContinousValue, evt) 
                  {
                     if(evt.startValueCanJump)
                     {
                        if(it == m_ongoingContinousEventStartPoints.end())
                        {
                           m_ongoingContinousEventStartPoints.push_back(event);
                           emitEventHappened(Event{derivedEvtId, RelativeValueType{0.0}});
                        }
                        else
                        {
                           const float diff = mpark::get<ContinousValueType>(event.value).value -
                                              mpark::get<ContinousValueType>(it->value).value;
                           if(std::fabs(diff) < VALUE_JUMP_THRESHOLD)
                           {
                              emitEventHappened(Event{derivedEvtId, RelativeValueType{diff}});
                           }
                           else
                           {
                              it->value = event.value;
                              emitEventHappened(Event{derivedEvtId, RelativeValueType{0.0}});
                           }
                        }
                     }                     
                  },
                  CASE(description::controller::EventIncremental, evt) 
                  {
                     if(it == m_ongoingContinousEventStartPoints.end())
                     {
                        m_ongoingContinousEventStartPoints.push_back(event);
                        emitEventHappened(Event{derivedEvtId, RelativeValueType{0.0}});
                     }
                     else
                     {
                        const auto increment = mpark::get<IncrementType>(event.value);
                        const float diff = increment.value / float(increment.resolution) ;
                        emitEventHappened(Event{derivedEvtId, RelativeValueType{diff}});
                     }
                  },
                  CASE_DEFAULT {}
               END_SWITCH
            }
         },
         CASE(description::controller::EventDerivedIncremental, derivedEvt)
         {
            if(derivedEvt.sourceEventIdx == event.id.eventId)
            {
               auto it = std::ranges::find_if(m_lastContOrRelEventValues, [&event](const Event& e){
                  return e.id == event.id;
               });
               SWITCH(eventDescr)
                  CASE(description::controller::EventContinousValue, evt) 
                  {
                     if(evt.startValueCanJump){
                        if(it == m_lastContOrRelEventValues.end())
                        {
                           m_ongoingContinousEventStartPoints.push_back(event);
                        }
                        else
                        {
                           const float diff = mpark::get<ContinousValueType>(event.value).value -
                                              mpark::get<ContinousValueType>(it->value).value;
                           if(std::fabs(diff) < VALUE_JUMP_THRESHOLD)
                           {
                              emitEventHappened(Event{derivedEvtId, IncrementType{DERIVED_INCREMENT_RESOLUTION, int(diff * DERIVED_INCREMENT_RESOLUTION)}});
                           }
                           it->value = event.value;
                        }
                     }
                  },
                  CASE(description::controller::EventRelativeValue, evt) 
                  {
                     if(it == m_lastContOrRelEventValues.end())
                     {
                        m_ongoingContinousEventStartPoints.push_back(event);
                     }
                     else
                     {
                        const float diff = mpark::get<RelativeValueType>(event.value).value -
                                             mpark::get<RelativeValueType>(it->value).value;
                        if(std::fabs(diff) < VALUE_JUMP_THRESHOLD)
                        {
                           emitEventHappened(Event{derivedEvtId, IncrementType{DERIVED_INCREMENT_RESOLUTION, int(diff * DERIVED_INCREMENT_RESOLUTION)}});
                        }
                        it->value = event.value;
                     }
                  },
                  CASE_DEFAULT {}
               END_SWITCH
            }
         },
         MFCASE_DEFAULT { eventIdx = -1; }
      END_SWITCH
   }
   emitEventHappened(event);
}