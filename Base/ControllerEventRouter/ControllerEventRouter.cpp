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

template <typename Dev, typename... DevCoord>
void playNoteOnOff(Dev& dev, int note, float velocity,
                   const DevCoord&... devCoord)
{
   if (velocity > 0)
   {
      dev.noteOn(devCoord..., note, velocity);
   }
   else
   {
      dev.noteOff(devCoord..., note, -velocity);
   }
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

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const ContinousValueType& value, MDCoords... mdCoords)
{
   const float val = dev.normalizePercentageValue(
       mdCoords..., parameter.id,
       base::musicDevice::sound::ParameterPart::Commanded, value.value);
   const float actualVal = dev.getParameterValue(mdCoords..., parameter.id);
   const float diff      = std::fabs(actualVal - val);
   if ((diff != 0) && (diff < 0.02 || diff >= 1.0))
   {
      dev.setParameterValue(mdCoords..., parameter.id, val);
   }
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const IncrementType& increment, MDCoords... mdCoords)
{
   float incr = 0;
   if (parameter.isList)
   {
      const int accIncr          = increment.value + parameter.storedIncrements;
      const int incrForOneStep   = increment.resolution / 12;
      incr                       = accIncr / incrForOneStep;
      parameter.storedIncrements = accIncr % incrForOneStep;
   }
   else
   {   // TODO: highres mode
      incr = float(increment.value) / float(increment.resolution);
   }
   dev.incrementParameterValue(mdCoords..., parameter.id, incr, false);
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const RelativeValueType& value, MDCoords... mdCoords)
{
   float valueToSet = value.value;
   if (!parameter.valueAtPress)
   {
      parameter.valueAtPress.emplace<float>(
          dev.getParameterValue(mdCoords..., parameter.id));
   }
   valueToSet += parameter.valueAtPress.value();
   dev.setParameterValue(mdCoords..., parameter.id, valueToSet);
   if (0 == value.value)
   {
      parameter.valueAtPress = std::nullopt;
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
                    handleAnyWidgetCoordPressRelease(widgetCoord,
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
                    handleAnyNotePressRelease(note.number, destIter2->second,
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
      playNoteOnOff(kitInstr, note.value, value.value, drumKit.voiceIdx);
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
   m_rMusicDeviceContainer.withSoundHandler(
       musicDevice.uuid, [&](auto& soundHandler) {
          playNoteOnOff(soundHandler, note.value, value.value,
                        musicDevice.voiceIdx);
       });
}

void EventRouter::setParameterOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Parameter& parameter,
    const PressReleaseType& value) noexcept

{
   m_rMusicDeviceContainer.withSoundHandler(
       musicDevice.uuid, [&](auto& soundHandler) {
          setParameter(soundHandler, parameter, value, musicDevice.voiceIdx);
       });
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

void EventRouter::playLayoutMappedDrumKit(
    const WidgetCoord& widgetCoord, EventDestination::DrumKit& drumKit,
    const PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      playNoteOnOff(kitInstr, 64, value.value,
                    widgetCoord.row * 8 + widgetCoord.col);
   });
}

void EventRouter::handleAnyWidgetCoordPressRelease(
    const WidgetCoord& widgetCoord, const EventDestination& eventDestination,
    const PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& note) {
                         playLayoutMappedDrumKit(widgetCoord, drumKit, value);
                      },
                      [](const EventDestination::Parameter& parameter) {},
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](EventDestination::Melodic& melodic) {},
           [](EventDestination::MusicDevice& musicDevice) {}, [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::handleAnyNotePressRelease(
    int note, const EventDestination& eventDestination,
    const PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& dstNote) {
                         if (drumKit.voiceIdx == ANY)
                         {
                            m_rInstruments.withKitInstrument(
                                drumKit.uuid, [&](auto& kitInstr) {
                                   playNoteOnOff(kitInstr, note, value.value);
                                });
                         }
                         else
                         {
                            m_rInstruments.withKitInstrument(
                                drumKit.uuid, [&](auto& kitInstr) {
                                   playNoteOnOff(kitInstr, note, value.value,
                                                 drumKit.voiceIdx);
                                });
                         }
                      },
                      [](const EventDestination::Parameter& parameter) {},
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& dstNote) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                playNoteOnOff(melodicInstr, note, value.value);
                             });
                      },
                      [](const EventDestination::Parameter& parameter) {},
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {
              mpark::visit(
                  util::overload{
                      [&, this](const EventDestination::Note& dstNote) {
                         m_rMusicDeviceContainer.withSoundHandler(
                             musicDevice.uuid, [&](auto& soundaHandler) {
                                playNoteOnOff(soundaHandler, note, value.value,
                                              musicDevice.voiceIdx);
                             });
                      },
                      [](const EventDestination::Parameter& parameter) {},
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::handleContinousValue(const EventDestination& eventDestination,
                                       const ContinousValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withKitInstrument(
                             drumKit.uuid, [&](auto& kitInstr) {
                                setParameter(kitInstr, parameter, value,
                                             drumKit.voiceIdx,
                                             drumKit.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, value,
                                             melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rMusicDeviceContainer.withSoundHandler(
                             musicDevice.uuid, [&](auto& soundHandler) {
                                setParameter(soundHandler, parameter, value,
                                             musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::sendMPEContinousValue(
    int note, const EventDestination& eventDestination,
    const ContinousValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [](EventDestination::DrumKit&) {},
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, value,
                                             note, melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](EventDestination::MusicDevice&) {}, [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::handleIncrement(const EventDestination& eventDestination,
                                  const IncrementType& increment) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withKitInstrument(
                             drumKit.uuid, [&](auto& kitInstr) {
                                setParameter(kitInstr, parameter, increment,
                                             drumKit.voiceIdx,
                                             drumKit.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, increment,
                                             melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rMusicDeviceContainer.withSoundHandler(
                             musicDevice.uuid, [&](auto& soundHandler) {
                                setParameter(soundHandler, parameter, increment,
                                             musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::sendMPEIncrementValue(
    int note, const EventDestination& eventDestination,
    const IncrementType& increment) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit&) {},
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, increment,
                                             note, melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {},
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::handleRelativeValue(const EventDestination& eventDestination,
                                      const RelativeValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [&, this](EventDestination::DrumKit& drumKit) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withKitInstrument(
                             drumKit.uuid, [&](auto& kitInstr) {
                                setParameter(kitInstr, parameter, value,
                                             drumKit.voiceIdx,
                                             drumKit.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, value,
                                             melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rMusicDeviceContainer.withSoundHandler(
                             musicDevice.uuid, [&](auto& soundHandler) {
                                setParameter(soundHandler, parameter, value,
                                             musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouter::sendMPERelativeValue(int note,
                                       const EventDestination& eventDestination,
                                       const RelativeValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [](EventDestination::DrumKit&) {},
           [&, this](EventDestination::Melodic& melodic) {
              mpark::visit(
                  util::overload{
                      [](const EventDestination::Note&) {},
                      [&, this](const EventDestination::Parameter& parameter) {
                         m_rInstruments.withMelodicInstrument(
                             melodic.uuid, [&](auto& melodicInstr) {
                                setParameter(melodicInstr, parameter, value,
                                             note, melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](EventDestination::MusicDevice&) {}, [](auto&&) {}},
       eventDestination.endpoint);
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
