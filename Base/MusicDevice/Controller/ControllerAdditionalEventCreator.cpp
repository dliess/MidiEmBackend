#include "ControllerAdditionalEventCreator.h"

#include "ControllerSection.h"

using namespace base::musicDevice;
using namespace base::musicDevice::controller;

AdditionalEventCreator::AdditionalEventCreator(
    const description::controller::Section& rControllerSection) :
    m_rControllerSection(rControllerSection)
{
}

template<typename EventDescr>
int determineMainEvtIdx(const EventDescr& evt, const EventId& eventId, 
                        const std::list<EventId>& independentPressList)
{
   if(evt.twin)
   {
      EventId indepPREvtId(eventId);
      indepPREvtId.eventId = evt.twin->indepPressEvtIdx;
      return std::ranges::find(independentPressList, indepPREvtId) != 
         independentPressList.end() ? evt.twin->twinEvtIdx : eventId.eventId;
   }
   return eventId.eventId;
}

inline
std::optional<int> getSourceEventIdx(const description::controller::Event& evtDescr)
{
   return R_SWITCH(evtDescr)
      FCASE(description::controller::EventDerivedRelativeValue, evtDescr) -> std::optional<int>
      {
         return evtDescr.sourceEventIdx;
      },
      FCASE(description::controller::EventDerivedIncremental, evtDescr) -> std::optional<int>
      {
         return evtDescr.sourceEventIdx;
      },
      CASE_DEFAULT -> std::optional<int> { return std::nullopt; }
   R_END_SWITCH
}

void AdditionalEventCreator::createFromContinousToRelative(const Event& event, int destEvtIdx)
{
   EventId destEvtId(event.id);
   destEvtId.eventId = destEvtIdx;
   auto it = std::ranges::find_if(m_ongoingContinousEventStartPoints, [&event](const Event& e){
      return e.id == event.id;
   });
   if(it == m_ongoingContinousEventStartPoints.end())
   {
      m_ongoingContinousEventStartPoints.push_back(event);
      emitEventHappened(Event{destEvtId, RelativeValueType{0.0}});
   }
   else
   {
      const float diff = mpark::get<ContinousValueType>(event.value).value -
                           mpark::get<ContinousValueType>(it->value).value;
      if(std::fabs(diff) < VALUE_JUMP_THRESHOLD)
      {
         emitEventHappened(Event{destEvtId, RelativeValueType{diff}});
      }
      else
      {
         it->value = event.value;
         emitEventHappened(Event{destEvtId, RelativeValueType{0.0}});
      }
   }
}

void AdditionalEventCreator::createFromContinousToIncremental(const Event& event, int destEvtIdx)
{
   EventId destEvtId(event.id);
   destEvtId.eventId = destEvtIdx;
   auto it = std::ranges::find_if(m_lastContOrRelEventValues, [&event](const Event& e){
      return e.id == event.id;
   });
   if(it == m_lastContOrRelEventValues.end())
   {
      m_lastContOrRelEventValues.push_back(event);
   }
   else
   {
      const float diff = mpark::get<ContinousValueType>(event.value).value -
                           mpark::get<ContinousValueType>(it->value).value;
      const int incr = int(diff * DERIVED_INCREMENT_RESOLUTION);
      if(std::fabs(diff) < VALUE_JUMP_THRESHOLD && incr != 0)
      {
         emitEventHappened(Event{destEvtId, IncrementType{DERIVED_INCREMENT_RESOLUTION, incr}});
      }
      it->value = event.value;
   }
}
void AdditionalEventCreator::createFromRelativeToIncremental(const Event& event, int destEvtIdx)
{
   EventId destEvtId(event.id);
   destEvtId.eventId = destEvtIdx;
   auto it = std::ranges::find_if(m_lastContOrRelEventValues, [&event](const Event& e){
      return e.id == event.id;
   });
   if(it == m_lastContOrRelEventValues.end())
   {
      m_lastContOrRelEventValues.push_back(event);
   }
   else
   {
      const float diff = mpark::get<RelativeValueType>(event.value).value -
                           mpark::get<RelativeValueType>(it->value).value;
      const int incr = int(diff * DERIVED_INCREMENT_RESOLUTION);
      if(std::fabs(diff) < VALUE_JUMP_THRESHOLD  && incr != 0)
      {
         emitEventHappened(Event{destEvtId, IncrementType{DERIVED_INCREMENT_RESOLUTION, incr}});
      }
      it->value = event.value;
   }
}

void AdditionalEventCreator::createFromIncrementalToRelative(const Event& event, int destEvtIdx)
{
   EventId destEvtId(event.id);
   destEvtId.eventId = destEvtIdx;
   auto it = std::ranges::find_if(m_ongoingContinousEventStartPoints, [&event](const Event& e){
      return e.id == event.id;
   });
   if(it == m_ongoingContinousEventStartPoints.end())
   {
      m_ongoingContinousEventStartPoints.push_back(event);
      emitEventHappened(Event{destEvtId, RelativeValueType{0.0}});
   }
   else
   {
      const auto increment = mpark::get<IncrementType>(event.value);
      const float diff = increment.value / float(increment.resolution) ;
      emitEventHappened(Event{destEvtId, RelativeValueType{diff}});
   }
}

void AdditionalEventCreator::createExtraEvents4ContinousEvent(const std::vector<description::controller::Event>& eventsDescr,
   const Event& event, int sourceEvtIdx)
{
   for(int destEvtIdx = sourceEvtIdx + 1; 
       destEvtIdx < eventsDescr.size() && 
       getSourceEventIdx(eventsDescr[destEvtIdx]).value_or(-1) == sourceEvtIdx; 
       ++destEvtIdx)
   {
      SWITCH(eventsDescr[destEvtIdx])
         FCASE(description::controller::EventDerivedRelativeValue, destEvtDescr)
         {
            createFromContinousToRelative(event, destEvtIdx);
         },
         FCASE(description::controller::EventDerivedIncremental, destEvtDescr)
         {
            createFromContinousToIncremental(event, destEvtIdx);
         },
         CASE_DEFAULT { }
      END_SWITCH
   }
}

void AdditionalEventCreator::createExtraEvents4RelativeEvent(const std::vector<description::controller::Event>& eventsDescr,
                     const Event& event, int sourceEvtIdx)
{
   for(int destEvtIdx = sourceEvtIdx + 1; 
       destEvtIdx < eventsDescr.size() && 
       getSourceEventIdx(eventsDescr[destEvtIdx]).value_or(-1) == sourceEvtIdx; 
       ++destEvtIdx)
   {
      SWITCH(eventsDescr[destEvtIdx])
         FCASE(description::controller::EventDerivedIncremental, destEvtDescr)
         {
            createFromRelativeToIncremental(event, destEvtIdx);
         },
         CASE_DEFAULT { }
      END_SWITCH
   }
}
void AdditionalEventCreator::createExtraEvents4IncrementalEvent(const std::vector<description::controller::Event>& eventsDescr,
                     const Event& event, int sourceEvtIdx)

{
   for(int destEvtIdx = sourceEvtIdx + 1; 
       destEvtIdx < eventsDescr.size() && 
       getSourceEventIdx(eventsDescr[destEvtIdx]).value_or(-1) == sourceEvtIdx; 
       ++destEvtIdx)
   {
      SWITCH(eventsDescr[destEvtIdx])
         FCASE(description::controller::EventDerivedRelativeValue, destEvtDescr)
         {
            createFromIncrementalToRelative(event, destEvtIdx);
         },
         CASE_DEFAULT { }
      END_SWITCH
   }
}

void AdditionalEventCreator::createExtraEvents4PressReleaseEvent(
   const description::controller::EventPressRelease& evtDescr,
   const Event& event)
{
   const auto value = mpark::get<PressReleaseType>(event.value).value;
   if(value > 0.0)
   {
      if(evtDescr.pressVelocityEvtIdx)
      {
         EventId derivedEvtId(event.id);
         derivedEvtId.eventId = evtDescr.pressVelocityEvtIdx.value();
         emitEventHappened(Event{derivedEvtId, ContinousValueType{value}});
      }
      if(evtDescr.independent.value_or(false))
      {
         m_independentPressList.push_back(event.id);
      }
   }
   if(value <= 0.0)
   {
      if(evtDescr.releaseVelocityEvtIdx)
      {
         EventId derivedEvtId(event.id);
         derivedEvtId.eventId = evtDescr.releaseVelocityEvtIdx.value();
         emitEventHappened(Event{derivedEvtId, ContinousValueType{-value}});
      }
      if(evtDescr.independent.value_or(false))
      {
         auto it = std::ranges::find(m_independentPressList, event.id);
         if(it != m_independentPressList.end())
         {
            m_independentPressList.erase(it);
         }
      }
   }
}

void AdditionalEventCreator::eventReceived(const Event& event)
{
   const auto& eventsDescr = m_rControllerSection.widgets[event.id.widgetId].events;
   const auto& eventDescr = eventsDescr[event.id.eventId];

   SWITCH(eventDescr)
      CASE(description::controller::EventPressRelease, evtDescr)
      {
         createExtraEvents4PressReleaseEvent(evtDescr, event);
      },
      CASE(description::controller::EventContinousValue, evtDescr)
      {
         const int evtIdx = determineMainEvtIdx(evtDescr, event.id, m_independentPressList);
         createExtraEvents4ContinousEvent(eventsDescr, event, evtIdx);
      },
      CASE(description::controller::EventRelativeValue, evtDescr)
      {
         const int evtIdx = determineMainEvtIdx(evtDescr, event.id, m_independentPressList);
         createExtraEvents4RelativeEvent(eventsDescr, event, evtIdx);
      },
      CASE(description::controller::EventIncremental, evtDescr)
      {
         const int evtIdx = determineMainEvtIdx(evtDescr, event.id, m_independentPressList);
         createExtraEvents4IncrementalEvent(eventsDescr, event, evtIdx);
      },
      CASE_DEFAULT {}
   END_SWITCH
   emitEventHappened(event);
}