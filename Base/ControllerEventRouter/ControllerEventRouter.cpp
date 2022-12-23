#include "ControllerEventRouter.h"

#include "ControllerHandler.h"
#include "Instruments.h"
#include "MusicDeviceContainer.h"
#include "Tracks.h"

using namespace base::musicDevice::controller;

EventRouter::EventRouter(session::Tracks& rTracks,
                         instruments::Instruments& rInstruments,
                         MusicDeviceContainer& rMusicDeviceContainer) :
    m_rTracks(rTracks),
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
   m_rMusicDeviceContainer.onControllerDevEventOccured(
       [this](const util::Identifiable::UUID uuid,
              const controller::Event& event) {
          const EventIdExt eventIdExt{uuid, event.id};
          mpark::visit(
              util::overload{
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
}

void EventRouter::createConnection(const EventIdExt& from,
                                   const EventDestination& to) noexcept
{
   m_map[from] = to;
   emitGotConnected(from, to);
}

void EventRouter::removeConnection(const EventIdExt& eventIdExt) noexcept
{
   auto it = m_map.find(eventIdExt);
   m_map.erase(it);
   emitGotErased(eventIdExt);
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const PressReleaseType& value, MDCoords... mdCoords)
{
   if (parameter.isList)
   {
      if (value.value > 0)
      {
         const float incr = parameter.upwards ? value.value : -value.value;
         dev.incrementParameterValue(mdCoords..., parameter.id, incr, true);
      }
   }
   else
   {
      if (value.value > 0)
      {
         const float actualVal =
             dev.getParameterValue(mdCoords..., parameter.id);
         if (std::fabs(actualVal - parameter.zeroVal) <
             std::numeric_limits<float>::epsilon())
         {
            if (parameter.valueAtPress)
            {
               dev.setParameterValue(mdCoords..., parameter.id,
                                     *parameter.valueAtPress);
            }
         }
         else
         {
            parameter.valueAtPress =
                dev.getParameterValue(mdCoords..., parameter.id);
            dev.setParameterValue(mdCoords..., parameter.id, parameter.zeroVal);
         }
      }
   }
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
                 handlePressRelease(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<WidgetCoord>(ANY,
                                                                       ANY);
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    handleWidgetCoordPressRelease(widgetCoord,
                                                  destIter2->second, value);
                 }
              }
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handlePressRelease(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{ANY});
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    handleNotePressRelease(note.number, destIter2->second,
                                           value);
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
                 handleContinousValue(destIter->second, value);
              }
              /*
              else
              {
                 spdlog::info("Not match found for event: {}\n in map:\n",
                              meta::serialize(eventIdExt).dump().c_str());
                  printMap();
              }
              */
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleContinousValue(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{ANY});
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
                 handleIncrement(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleIncrement(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{ANY});
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    sendMPEIncrementValue(note.number, destIter2->second,
                                          value);
                 }
              }
           },
           [this](auto&&) { assert(false); }},
       eventIdExt.eventId.widgetCoord);
}

void EventRouter::handleRelativeValueType(
    const EventIdExt& eventIdExt, const RelativeValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt, &value](const WidgetCoord& widgetCoord) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleRelativeValue(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const Note& note) {
              const auto destIter = m_map.find(eventIdExt);
              if (destIter != m_map.end())
              {
                 handleRelativeValue(destIter->second, value);
              }
              else
              {
                 EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<Note>(Note{ANY});
                 const auto destIter2 = m_map.find(melodicEvent);
                 if (destIter2 != m_map.end())
                 {
                    sendMPERelativeValue(note.number, destIter2->second, value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouter::playNoteOnDrumKit(const EventDestination::DrumKit& drumKit,
                                    const EventDestination::Note& note,
                                    const PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      if (value.value > 0)
      {
         kitInstr.noteOn(drumKit.voiceIdx, note.value, value.value);
      }
      else
      {
         kitInstr.noteOff(drumKit.voiceIdx, note.value, -value.value);
      }
   });
}

void EventRouter::setParameterOnDrumKit(
    const EventDestination::DrumKit& drumKit,
    const EventDestination::Parameter& parameter,
    const PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      setParameter(kitInstr, parameter, value, drumKit.voiceIdx,
                   drumKit.componentIdx);
   });
}

void EventRouter::setParameterOnMelodic(
    const EventDestination::Melodic& melodic,
    const EventDestination::Parameter& parameter,
    const PressReleaseType& value) noexcept
{
   m_rInstruments.withMelodicInstrument(melodic.uuid, [&](auto& melodicInstr) {
      setParameter(melodicInstr, parameter, value, melodic.componentIdx);
   });
}

void EventRouter::playNoteOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Note& note, const PressReleaseType& value) noexcept
{
   const auto mdIter = m_rMusicDeviceContainer.find(musicDevice.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      if (value.value > 0)
      {
         mdIter->second->soundHandler->noteOn(musicDevice.voiceIdx, note.value,
                                              value.value);
      }
      else
      {
         mdIter->second->soundHandler->noteOff(musicDevice.voiceIdx, note.value,
                                               -value.value);
      }
   }
}

void EventRouter::setParameterOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Parameter& parameter,
    const PressReleaseType& value) noexcept

{
   const auto mdIter = m_rMusicDeviceContainer.find(musicDevice.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      setParameter(*mdIter->second->soundHandler, parameter, value,
                   musicDevice.voiceIdx);
   }
}

void EventRouter::handlePressRelease(const EventDestination& eventDestination,
                                     const PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](const EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& note) {
                         playNoteOnDrumKit(drumKit, note, value);
                      },
                      [&, this](const EventDestination::Parameter& parameter) {
                         setParameterOnDrumKit(drumKit, parameter, value);
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](const EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& note) {
                         // NOTHING TO DO
                      },
                      [&, this](const EventDestination::Parameter& parameter) {
                         setParameterOnMelodic(melodic, parameter, value);
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](const EventDestination::MusicDevice& musicDevice) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& note) {
                         playNoteOnMusicDevice(musicDevice, note, value);
                      },
                      [&, this](const EventDestination::Parameter& parameter) {
                         setParameterOnMusicDevice(musicDevice, parameter,
                                                   value);
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
       },
       eventDestination.endpoint);
}

void EventRouter::handleWidgetCoordPressRelease(
    const WidgetCoord& widgetCoord, const EventDestination& eventDestination,
    const PressReleaseType& value) noexcept
{
   /*
      mpark::visit(util::overload{
                       [](EventDestination::DrumKit& drumKit) {},
                       [](EventDestination::Melodic& drumKit) {},
                       [](EventDestination::MusicDevice& drumKit) {},
                   },
                   eventDestination.endpoint);
   */
}

void EventRouter::handleNotePressRelease(
    int note, const EventDestination& eventDestination,
    const PressReleaseType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   m_rTracks.withTrack(eventDestination.uuid,
                       [&value, note, this](auto& track) {
                          if (value.value > 0)
                          {
                             track.noteOn(note, value.value);
                          }
                          else
                          {
                             track.noteOff(note, value.value);
                          }
                       });
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
   */
}

void EventRouter::handleContinousValue(const EventDestination& eventDestination,
                                       const ContinousValueType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{
              [&mdIter, &eventDestination,
               &value](const EventDestination::Parameter& parameter) {
                 const float val =
                     mdIter->second->soundHandler->normalizePercentageValue(
                         eventDestination.voiceIdx, parameter.id,
                         sound::ParameterPart::Commanded, value.value);
                 const float actualVal =
                     mdIter->second->soundHandler->getParameterValue(
                         eventDestination.voiceIdx, parameter.id);
                 const float diff = std::fabs(actualVal - val);
                 if ((diff != 0) && (diff < 0.02 || diff >= 1.0))
                 {
                    mdIter->second->soundHandler->setParameterValue(
                        eventDestination.voiceIdx, parameter.id, val);
                 }
              },
              [](auto&&) { assert(false); }},
          eventDestination.controlType);
   }
   */
}

void EventRouter::sendMPEContinousValue(
    int note, const EventDestination& eventDestination,
    const ContinousValueType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{[&mdIter, &note, &value](
                             const EventDestination::Parameter& parameter) {
                            // TODO:
                            //
   mdIter->second->soundHandler->setMPEParameterValue(
                            //     note, parameter.id, value.value);
                         },
                         [](auto&&) { assert(false); }},
          eventDestination.controlType);
   }
   */
}

void EventRouter::sendMPEIncrementValue(
    int note, const EventDestination& eventDestination,
    const IncrementType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);
                */
}

void EventRouter::handleIncrement(const EventDestination& eventDestination,
                                  const IncrementType& increment) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{
              [&mdIter, &eventDestination,
               &increment](const EventDestination::Parameter& parameter) {
                 float incr = 0;
                 if (parameter.isList)
                 {
                    const int accIncr =
                        increment.value + parameter.storedIncrements;
                    const int incrForOneStep   = increment.resolution / 12;
                    incr                       = accIncr / incrForOneStep;
                    parameter.storedIncrements = accIncr % incrForOneStep;
                 }
                 else
                 {   // TODO: highres mode
                    incr = float(increment.value) / float(increment.resolution);
                 }
                 mdIter->second->soundHandler->incrementParameterValue(
                     eventDestination.voiceIdx, parameter.id, incr);
              },
              [&increment](const EventDestination::Note& note) {
                 note.value += increment.value;
              },
              [](auto&& e) { assert(false); }},
          eventDestination.controlType);
   }
   */
}

void EventRouter::handleRelativeValue(const EventDestination& eventDestination,
                                      const RelativeValueType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{
              [&mdIter, &eventDestination,
               &value](const EventDestination::Parameter& parameter) {
                 float valueToSet = value.value;
                 if (!parameter.valueAtPress)
                 {
                    parameter.valueAtPress.emplace<float>(
                        mdIter->second->soundHandler->getParameterValue(
                            eventDestination.voiceIdx, parameter.id));
                 }
                 valueToSet += parameter.valueAtPress.value();
                 mdIter->second->soundHandler->setParameterValue(
                     eventDestination.voiceIdx, parameter.id, valueToSet);
                 if (0 == value.value)
                 {
                    parameter.valueAtPress = std::nullopt;
                 }
              },
              [](auto&&) { assert(false); }},
          eventDestination.controlType);
   }
   */
}

void EventRouter::sendMPERelativeValue(int note,
                                       const EventDestination& eventDestination,
                                       const RelativeValueType& value) noexcept
{
   /*
   mpark::visit(util::overload{
                    [](EventDestination::DrumKit& drumKit) {},
                    [](EventDestination::Melodic& drumKit) {},
                    [](EventDestination::MusicDevice& drumKit) {},
                },
                eventDestination.endpoint);

   // TODO !!!
   const auto mdIter = m_rMusicDeviceContainer.find(eventDestination.uuid);
   if (mdIter != m_rMusicDeviceContainer.end() && mdIter->second->soundHandler)
   {
      mpark::visit(
          util::overload{[&mdIter, &note, &value](
                             const EventDestination::Parameter& parameter) {
                            // TODO:
                            //
   mdIter->second->soundHandler->setMPEParameterValue(
                            //     note, parameter.id, value.value);
                         },
                         [](auto&&) { assert(false); }},
          eventDestination.controlType);
   }
   */
}

void EventRouter::printMap() const noexcept
{
   for (const auto& e : m_map)
   {
      spdlog::info("{}", meta::serialize(e.first).dump().c_str());
   }
}

void EventRouter::retriggerCallbacks()
{
   for (auto& e : m_map) { emitGotConnected(e.first, e.second); }
}
