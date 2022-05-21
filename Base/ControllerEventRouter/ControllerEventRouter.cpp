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
         const EventExt eventExt{uuid, event.id, event.value};
         mpark::visit(
             util::overload{[this, &eventExt](const PressReleaseType& value) {
                               handlePressReleaseType(eventExt, value);
                            },
                            [this, &eventExt](const ContinousValueType& value) {
                               handleContinousValueType(eventExt, value);
                            },
                            [this, &eventExt](const IncrementType& value) {
                               handleIncrementType(eventExt, value);
                            },
                            [this, &eventExt](const RelativeValueType& value) {
                               handleRelativeValueType(eventExt, value);
                            },
                            [this](auto&&) {}},
             event.value);
      });
   });
}

void EventRouter::handlePressReleaseType(const EventExt& eventExt,
                                         const PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventExt, &value](const WidgetCoord& widgetCoord) {
              const auto destIter = m_map.find(eventExt);
              if (destIter != m_map.end())
              {
                 // handlePressReleaseDirect();
                 const auto mdIter =
                     m_rMusicDeviceContainer.find(destIter->second.uuid);
                 if (mdIter != m_rMusicDeviceContainer.end() &&
                     mdIter->second->soundHandler)
                 {
                    mpark::visit(
                        util::overload{
                            [&mdIter, &destIter, &eventExt,
                             &value](const EventDestination::Note& note) {
                               if (value.value > 0)
                               {
                                  mdIter->second->soundHandler->noteOn(
                                      destIter->second.voiceIdx, note.value,
                                      value.value);
                               }
                               else
                               {
                                  mdIter->second->soundHandler->noteOff(
                                      destIter->second.voiceIdx, note.value,
                                      -value.value);
                               }
                            },
                            [&mdIter, &destIter, &eventExt,
                             &value](const EventDestination::ParameterId&
                                   parameterId) {
                               mdIter->second->soundHandler->        
                               if (value.value > 0)
                               {
                                  mdIter->second->soundHandler->noteOn(
                                      destIter->second.voiceIdx, note.value,
                                      value.value);
                               }
                            },
                            [](const EventDestination::InternalFunctionality&
                                   internalFunct) {},
                            [](auto&&) {}},
                        destIter->second.endpoint);
                 }
              }
           },
           [this, &eventExt](const Note& note) {
              const auto iter = m_map.find(eventExt);
              if (iter != m_map.end())
              {
                 // handlePressReleaseDirect();
              }
              else
              {
                 EventExt melodicEvent = eventExt;
                 melodicEvent.event.id.widgetCoord.emplace<Note>(Note{-1});
                 const auto iter2 = m_map.find(melodicEvent);
                 if (iter2 != m_map.end())
                 {
                    // sendNoteOnOff(*iter2, ...);
                 }
              }
           },
           [this](auto&&) {}},
       eventExt.event.id.widgetCoord);
}

void EventRouter::handleContinousValueType(
    const EventExt& event, const ContinousValueType& value) noexcept
{
}

void EventRouter::handleIncrementType(const EventExt& event,
                                      const IncrementType& value) noexcept
{
}

void EventRouter::handleRelativeValueType(
    const EventExt& event, const RelativeValueType& value) noexcept
{
}
