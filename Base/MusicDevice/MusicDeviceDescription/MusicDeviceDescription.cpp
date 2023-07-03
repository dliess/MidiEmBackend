#include "MusicDeviceDescription.h"

#include <fmt/format.h>

#include <cassert>
#include <exception>

#include "2DMatrixCheck.h"
#include "Overload.h"



using namespace base::musicDevice::description;

template <typename T>
void checkSource(const std::vector<std::vector<T>>& matrix2D,
                 const controller::WidgetDimension& widgetDim,
                 const std::string& eventName)
{
   const auto [numRows, numCols] = util::dimOf(matrix2D);
   if (numRows == 0 || numCols == 0)
   {
      throw std::runtime_error(
          fmt::format("dimension is 0 in source of event '{}'", eventName));
   }
   if (numRows > 1 || numCols > 1)
   {
      if (numRows != widgetDim.numRows)
      {
         throw std::runtime_error(fmt::format(
             "row dimension mismatch between widget and source of event '{}'",
             eventName));
      }
      if (numCols != widgetDim.numColumns)
      {
         throw std::runtime_error(
             fmt::format("column dimension mismatch between widget and source "
                         "of event '{}'",
                         eventName));
      }
      if (!util::isConsistent(matrix2D))
      {
         throw std::runtime_error(fmt::format(
             "source of event '{}' is an inconsistent matrix", eventName));
      }
   }
}

void Description::checkValidity() const
{
   if (controllerSection)
   {
      for (const auto& widget : controllerSection->widgets)
      {
         for (const auto& event : widget.events)
         {
            SWITCH(event)
               CASE(controller::EventPressRelease, evt) 
               {
                  checkSource(evt.pressSource, widget.dimension, evt.name);
                  checkSource(evt.releaseSource, widget.dimension,
                              evt.name);
               },
               CASE(controller::EventContinousValue, evt) 
               {
                  checkSource(evt.source, widget.dimension, evt.name);
               },
               CASE(controller::EventRelativeValue, evt)
               {
                  checkSource(evt.source, widget.dimension, evt.name);
               },
               CASE(controller::EventIncremental, evt)
               {
                  checkSource(evt.source, widget.dimension, evt.name);
               },
               CASE_DEFAULT {}
            END_SWITCH
         }
      }
   }
}

void Description::initCaches() noexcept
{
   if (soundSection)
   {
      soundSection->handleEngineInheritance();
      // For every engine
      for (auto& engine : soundSection->engines)
      {
         // Note pitch mapping
         if (engine.noteSettings && engine.noteSettings->midi)
         {
            if (engine.noteSettings->midi->pitchRouting)
            {
               const int idx = sound::Section::linSearchByName(
                   engine.parameters, engine.noteSettings->midi->pitchRouting
                                          ->destinationParameter);
               if (idx != -1)
               {
                  engine.noteSettings->midi->pitchRouting
                      ->destinationParameterIdx = idx;
               }
            }
            if (engine.noteSettings->midi->velocityRouting)
            {
               const int idx = sound::Section::linSearchByName(
                   engine.parameters, engine.noteSettings->midi->velocityRouting
                                          ->destinationParameter);
               if (idx != -1)
               {
                  engine.noteSettings->midi->velocityRouting
                      ->destinationParameterIdx = idx;
               }
            }
         }
      }
      soundSection->forEachParameterDescr([this](
                                              const sound::ParameterId& paramId,
                                              sound::Parameter& parameter) {
         if (parameter.source.midi && parameter.source.midi->sourceRangesFrom)
         {
            assert(soundSection->sourceRanges);
            auto it = soundSection->sourceRanges->find(
                *parameter.source.midi->sourceRangesFrom);
            assert(it != soundSection->sourceRanges->end());
            if (!parameter.source.midi->sourceRanges)
            {
               parameter.source.midi->sourceRanges.emplace();
            }
            parameter.source.midi->sourceRanges->insert(
                parameter.source.midi->sourceRanges->begin(),
                it->second.begin(), it->second.end());
         }
      });
      soundSection->autoFillSourceRangesForLists();
      soundSection->fillParameterDumpOffsetCaches();
   }
}

std::optional<int> createAdditionalControllerEventsForPressRelease(controller::Widget& widget)
{
   std::optional<int> indepPressEvtIdx;
   for (int eventIdx = 0; eventIdx < widget.events.size(); ++eventIdx)
   {
      SWITCH(widget.events[eventIdx])
         MFCASE(controller::EventPressRelease, evt){
            if(evt.independent.value_or(false))
            {
               if(indepPressEvtIdx)
               {
                  spdlog::error("Only one independent PressRelease Evt allowed per widget");
               }
               else
               {
                  indepPressEvtIdx = eventIdx;
               }
            }
            int step = 1;
            if(evt.hasPressVelocity.value_or(false))
            {
               const controller::Event event1 = controller::EventDerivedContinousValue{
                  "PressVelocity", eventIdx};
               evt.pressVelocityEvtIdx = eventIdx + step;
               widget.events.insert(widget.events.begin() + eventIdx + step, event1);
               ++step;
            }
            if(evt.hasReleaseVelocity.value_or(false))
            {
               const controller::Event event2 = controller::EventDerivedContinousValue{
                  "ReleaseVelocity", eventIdx};
               evt.releaseVelocityEvtIdx = eventIdx + step;
               widget.events.insert(widget.events.begin() + eventIdx + step, event2);
               ++step;
            }
            auto willHaveNoteCoordinates = [&]() {
               return (evt.pressSource.size() * evt.pressSource[0].size()) != 
                      (widget.dimension.numColumns * widget.dimension.numRows);
            };
            if(willHaveNoteCoordinates())
            {
               const controller::Event event3 = controller::EventDerivedContinousValue{
                  "Keytrack", eventIdx};
               evt.keytrackEvtIdx = eventIdx + step;
               widget.events.insert(widget.events.begin() + eventIdx + step, event3);
            }
         },
         CASE_DEFAULT {}
      END_SWITCH
   }
   return indepPressEvtIdx;
}

void createAdditionalControllerDerivedEvents(controller::Widget& widget)
{
   for (int eventIdx = 0; eventIdx < widget.events.size(); ++eventIdx)
   {
      SWITCH(widget.events[eventIdx])
         MFCASE(controller::EventContinousValue, evt)
         {            
            if (evt.startValueCanJump.value_or(false))
            {
               const controller::Event event1 = controller::EventDerivedRelativeValue{
                  fmt::format("{}_Relative", evt.name), eventIdx};
               widget.events.insert(widget.events.begin() + eventIdx + 1, event1);
               const controller::Event event2 = controller::EventDerivedIncremental{
                  fmt::format("{}_Incremental", evt.name), eventIdx};
               widget.events.insert(widget.events.begin() + eventIdx + 2, event2);
            }
            
         },
         MFCASE(controller::EventRelativeValue, evt)
         {
            if(!evt.springsToZeroAtRelease)
            {
               const controller::Event event = controller::EventDerivedIncremental{
                  fmt::format("{}_Incremental", evt.name), eventIdx};
               widget.events.insert(widget.events.begin() + eventIdx + 1, event);
            }
         },
         MFCASE(controller::EventIncremental, evt)
         {
            if(controller::getDependentPressReleaseEventIdx(widget).has_value())
            {
               const controller::Event event = controller::EventDerivedRelativeValue{
                  fmt::format("{}_Relative", evt.name), eventIdx};
               widget.events.insert(widget.events.begin() + eventIdx + 1, event);
            }
         },
         CASE_DEFAULT {}
      END_SWITCH
   }
}

void createAdditionalControllerTwinEvents(controller::Widget& widget, int indepPressEvtIdx)
{
   for (int eventIdx = 0; eventIdx < widget.events.size(); ++eventIdx)
   {
      SWITCH(widget.events[eventIdx])
         MFCASE(controller::EventContinousValue, evt)
         {
            evt.twin = controller::TwinData{indepPressEvtIdx, 
                                          int(widget.events.size())};
            auto twin = evt;
            twin.name.append("_Alt");
            widget.events.emplace_back(twin);
         },
         MFCASE(controller::EventRelativeValue, evt)
         {
            evt.twin = controller::TwinData{indepPressEvtIdx, 
                                          int(widget.events.size())};
            auto twin = evt;
            twin.name.append("_Alt");
            widget.events.emplace_back(twin);
         },
         MFCASE(controller::EventIncremental, evt)
         {
            evt.twin = controller::TwinData{indepPressEvtIdx, 
                                          int(widget.events.size())};
            auto twin = evt;
            twin.name.append("_Alt");
            widget.events.emplace_back(twin);
         },
         CASE_DEFAULT {}
      END_SWITCH
   }
}

void Description::createAdditionalControllerEvents()
{
   if (controllerSection)
   {
      for (auto& widget : controllerSection->widgets)
      {
         widget.events.reserve(16);
         const std::optional<int> indepPressEvtIdx =
            createAdditionalControllerEventsForPressRelease(widget);
         createAdditionalControllerDerivedEvents(widget);
         if(indepPressEvtIdx)
         {
            createAdditionalControllerTwinEvents(widget, indepPressEvtIdx.value());
         }
         for(const auto& event : widget.events)
         {
            std::string name = 
            R_SWITCH(event)
               [](auto&& evt) -> std::string { return evt.name; }
            R_END_SWITCH
            spdlog::info("{}", name);
         }
      }
   }
}