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

inline std::optional<int> getPressReleaseEventIdx(
    const controller::Widget& widget)
{
   for (int i = 0; i < widget.events.size(); ++i)
   {
      if (mpark::holds_alternative<controller::EventPressRelease>(
              widget.events[i]))
      {
         return i;
      }
   }
   return std::nullopt;
}

void Description::createAdditionalControllerEvents()
{
   if (controllerSection)
   {
      for (auto& widget : controllerSection->widgets)
      {
         std::optional<int> indepPressEvtIdx;
         for (int eventIdx = 0; eventIdx < widget.events.size(); ++eventIdx)
         {
            SWITCH(widget.events[eventIdx])
               MCASE(controller::EventPressRelease, evt){
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
                  if(evt.hasPressVelocity.value_or(false))
                  {
                     const controller::Event event1 = controller::EventDerivedContinousValue{
                        "PressVelocity", eventIdx};
                     evt.pressVelocityEvtIdx = widget.events.size();
                     widget.events.push_back(event1);
                  }
                  if(evt.hasReleaseVelocity.value_or(false))
                  {
                     const controller::Event event2 = controller::EventDerivedContinousValue{
                        "ReleaseVelocity", eventIdx};
                     evt.releaseVelocityEvtIdx = widget.events.size();
                     widget.events.push_back(event2);
                  }
               },
               CASE_DEFAULT {}
            END_SWITCH
         }
         for (int eventIdx = 0; eventIdx < widget.events.size(); ++eventIdx)
         {
            SWITCH(widget.events[eventIdx])
               MCASE(controller::EventContinousValue, evt)
               {
                  if(indepPressEvtIdx)
                  {
                     evt.twin = controller::TwinData{indepPressEvtIdx.value(), 
                                                     int(widget.events.size())};
                     auto twin = evt;
                     twin.name.append("Alt");
                     widget.events.emplace_back(twin);
                  }
                  
                  if (evt.startValueCanJump.value_or(false))
                  {
                     const controller::Event event = controller::EventDerivedRelativeValue{
                        fmt::format("{}_Relative", evt.name), eventIdx};
                     widget.events.push_back(event);
                  }
                  
               },
               MCASE(controller::EventRelativeValue, evt)
               {

               },
               MCASE(controller::EventIncremental, evt)
               {
                  
               },
               CASE_DEFAULT {}
            END_SWITCH
         }
      }
   }
}