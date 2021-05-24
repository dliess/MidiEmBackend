#include "MusicDeviceDescription.h"

#include <fmt/format.h>

#include <cassert>
#include <exception>

#include "2DMatrixCheck.h"
#include "Overload.h"

using namespace base::musicDevice::description;

template<typename T>
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
            "row dimension mismatch between widget and source of event '{}'", eventName));
      }
      if (numCols != widgetDim.numColumns)
      {
         throw std::runtime_error(fmt::format(
            "column dimension mismatch between widget and source of event '{}'", eventName));
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
            mpark::visit(
               util::overload{
                  [&widget](const controller::EventPressRelease& evt) {
                     checkSource(evt.pressSource, widget.dimension, evt.name);
                     checkSource(evt.releaseSource, widget.dimension, evt.name);
                  },
                  [&widget](const controller::EventContinousValue& evt) {
                     checkSource(evt.source, widget.dimension, evt.name);
                  },
                  [&widget](const controller::EventRelativeValue& evt) {
                     checkSource(evt.source, widget.dimension, evt.name);
                  },
                  [&widget](const controller::EventIncremental& evt) {
                     checkSource(evt.source, widget.dimension, evt.name);
                  }},
               event);
         }
      }
   }
}

void Description::initCaches() noexcept
{
   if(soundSection)
   {
      // For global section
      if(soundSection->global)
      {
         for(auto& [paramName, paramDescr] : soundSection->global->parameters)
         {
            paramDescr.name = paramName;
            soundSection->global->parametersCached.push_back(paramDescr);
         }
         if(soundSection->global->components)
         {
            for(auto& [componentName, componentDescr] : *soundSection->global->components)
            {
               componentDescr.name = componentName;
               soundSection->global->componentsCached.push_back(componentDescr);
            }
         }
      }

      // For every engine
      for(auto& engine : soundSection->engines)
      {
         int j = 0;
         for(auto& [paramName, paramDescr] : engine.parameters)
         {
            paramDescr.name = paramName;
            paramDescr.index = j++;
            engine.parametersCached.push_back(paramDescr);
         }
         j = 0;
         if(engine.components)
         {
            for(auto& [componentName, componentDescr] : *engine.components)
            {
               componentDescr.name = componentName;
               componentDescr.index = j++;
               engine.componentsCached.push_back(componentDescr);
            }
         }
         // Note pitch mapping
         if(engine.noteSettings && engine.noteSettings->midi)
         {
            if(engine.noteSettings->midi->pitchRouting)
            {
               const auto iter = engine.parameters.find(
                  engine.noteSettings->midi->pitchRouting->destinationParameter
               );
               if(iter != engine.parameters.end())
               {
                  assert(iter->second.index);
                  engine.noteSettings->midi->pitchRouting->destinationParameterIdx =
                     *iter->second.index;
               }
            }
            if(engine.noteSettings->midi->velocityRouting)
            {
               const auto iter = engine.parameters.find(
                  engine.noteSettings->midi->velocityRouting->destinationParameter
               );
               if(iter != engine.parameters.end())
               {
                  assert(iter->second.index);
                  engine.noteSettings->midi->velocityRouting->destinationParameterIdx =
                     *iter->second.index;
               }
            }
         }
      }
   }
}