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
            mpark::visit(
                util::overload{
                    [&widget](const controller::EventPressRelease& evt) {
                       checkSource(evt.pressSource, widget.dimension, evt.name);
                       checkSource(evt.releaseSource, widget.dimension,
                                   evt.name);
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
   handleInheritance();
   if (soundSection)
   {
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
   }
}

#define VAR_ACCESS(variant, member) \
   mpark::visit(                    \
      util::overload{               \
         [](auto&& v) -> std::string { return v.member; }}, (variant))

void Description::handleInheritance() noexcept
{
   if (!soundSection)
      return;

   for (auto& engine : soundSection->engines)
   {
      if (!engine.from)
         continue;
      assert(soundSection->engineTemplates);
      const auto baseEngineIter =
          soundSection->engineTemplates->find(*engine.from);
      assert(baseEngineIter != soundSection->engineTemplates->end());
      sound::Engine destEngine = baseEngineIter->second;
      destEngine.name          = engine.name;
      if (engine.noteSettings)
      {
         destEngine.noteSettings = engine.noteSettings;
      }
      if (engine.components)
      {
         for (const auto& childComponent : *engine.components)
         {
            if (destEngine.components)
            {
               auto it = std::find_if(
                   destEngine.components->begin(), destEngine.components->end(),
                   [childComponent](const sound::ComponentVar& c) {
                      const std::string name1 = VAR_ACCESS(c, name);
                      /*const std::string name1 = mpark::visit(
                          util::overload{
                              [](auto&& v) -> std::string { return v.name; }},
                          c);*/
                      const std::string name2 = mpark::visit(
                          util::overload{
                              [](auto&& v) -> std::string { return v.name; }},
                          childComponent);
                      return name1 == name2;
                   });
               if (it != std::end(*destEngine.components))
               {
                  *it = childComponent;
               }
               else
               {
                  destEngine.components->push_back(childComponent);
               }
            }
            //TODO: params
         }
      }
   }
}
