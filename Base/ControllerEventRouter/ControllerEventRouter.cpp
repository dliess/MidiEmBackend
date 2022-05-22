#include "ControllerEventRouter.h"

#include "ControllerHandler.h"
#include "MusicDeviceContainer.h"

using namespace base::musicDevice::controller;

EventRouter::EventRouter(MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
   m_rMusicDeviceContainer.onAdded([this](std::shared_ptr<MusicDevice> pMd) {
      if (!pMd->controllerHandler)
      {
         return;
      }
      const auto uuid = pMd->id();
      pMd->controllerHandler->onEventReceived([this, uuid](const Event& event) {
         const EventIdExt eventIdExt{uuid, event.id};
         mpark::visit(util::overload{
                          [this, &eventIdExt](const PressReleaseType& value) {
                             handlePressReleaseType(eventIdExt, value);
                          },
                          [this, &eventIdExt](const ContinousValueType& value) {
                             handleContinousValueType(eventIdExt, value);
                          },
                          [this, &eventIdExt](const IncrementType& value) {
                             handleIncrementType(eventIdExt, value);
                          },
                          [this, &eventIdExt](const RelativeValueType& value) {
                             handleRelativeValueType(eventIdExt, value);
                          },
                          [this](auto&&) {}},
                      event.value);
      });
   });
}

void EventRouter::handlePressReleaseType(const EventIdExt& eventIdExt,
                                         const PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt, &value](const WidgetCoord& widgetCoord) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handlePressReleaseDirect(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handlePressReleaseDirect(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{-1});
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    sendNoteOnOff(note.number, destIter2->second, value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouter::handleContinousValueType(
    const EventIdExt& eventIdExt, const ContinousValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt, &value](const WidgetCoord& widgetCoord) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleContinousValueDirect(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleContinousValueDirect(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{-1});
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    sendMPEContinousValue(note.number, destIter2->second,
                                          value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouter::handleIncrementType(const EventIdExt& eventIdExt,
                                      const IncrementType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt, &value](const WidgetCoord& widgetCoord) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleIncrementDirect(destIter->second, value);
              }
           },
           [this](auto&&) { assert(false); }},
       eventIdExt.eventId.widgetCoord);
}

void EventRouter::handleRelativeValueType(
    const EventIdExt& event, const RelativeValueType& value) noexcept
{
}

void EventRouter::handlePressReleaseDirect(
    const EventDestination& eventDestination,
    const PressReleaseType& value) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{
              [&mdIter, &eventDestination,
               &value](const EventDestination::Note& note) {
                 if (value.value > 0)
                 {
                    mdIter->second->soundHandler->noteOn(
                        eventDestination.voiceIdx, note.value, value.value);
                 }
                 else
                 {
                    mdIter->second->soundHandler->noteOff(
                        eventDestination.voiceIdx, note.value, -value.value);
                 }
              },
              [&mdIter, &eventDestination,
               &value](const EventDestination::Parameter& parameter) {
                 if (value.value > 0)
                 {
                    const float incr =
                        parameter.upwards ? value.value : -value.value;
                    mdIter->second->soundHandler->incrementParameterValue(
                        eventDestination.voiceIdx, parameter.id, incr);
                 }
              },
              [](const EventDestination::InternalFunctionality& internalFunct) {
              },
              [](auto&&) {}},
          eventDestination.endpoint);
   }
}

void EventRouter::sendNoteOnOff(int note,
                                const EventDestination& eventDestination,
                                const PressReleaseType& value) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      if (value.value > 0)
      {
         mdIter->second->soundHandler->noteOn(eventDestination.voiceIdx, note,
                                              value.value);
      }
      else
      {
         mdIter->second->soundHandler->noteOff(eventDestination.voiceIdx, note,
                                               -value.value);
      }
   }
}

void EventRouter::handleContinousValueDirect(
    const EventDestination& eventDestination,
    const ContinousValueType& value) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{[&mdIter, &eventDestination, &value](
                             const EventDestination::Parameter& parameter) {
                            mdIter->second->soundHandler->setParameterValue(
                                eventDestination.voiceIdx, parameter.id,
                                value.value);
                         },
                         [](auto&&) { assert(false); }},
          eventDestination.endpoint);
   }
}

void EventRouter::sendMPEContinousValue(
    int note, const EventDestination& eventDestination,
    const ContinousValueType& value) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{[&mdIter, &note, &value](
                             const EventDestination::Parameter& parameter) {
                            // TODO:
                            // mdIter->second->soundHandler->setMPEParameterValue(
                            //     note, parameter.id, value.value);
                         },
                         [](auto&&) { assert(false); }},
          eventDestination.endpoint);
   }
}

void EventRouter::handleIncrementDirect(
    const EventDestination& eventDestination,
    const IncrementType& increment) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{[&mdIter, &eventDestination, &increment](
                             const EventDestination::Parameter& parameter) {
                            float incr = 0;
                            if(parameter.isList)
                            {
                               incr = increment.value * 12 / std::max(increment.resolution, 12);
                            }
                            else
                            { // TODO: highres mode
                               incr = increment.value / increment.resolution;
                            }
                            mdIter->second->soundHandler->incrementParameterValue(
                                eventDestination.voiceIdx, parameter.id,
                                incr);
                         },
                         [](auto&&) { assert(false); }},
          eventDestination.endpoint);
   }
}