#include "ControllerEventRouterRt.h"

#include "ControllerHandler.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"

using namespace base::eventRouter;
using namespace base::musicDevice;

EventRouterRt::EventRouterRt(const MapType& rMap,
                             ParameterCacheMap& rParameterCacheMap,
                             instruments::InstrumentsRef rInstruments,
                             MusicDeviceContainerRef rMusicDeviceContainer) :
    m_rMap(rMap),
    m_rParameterCacheMap(rParameterCacheMap),
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void EventRouterRt::onControllerDevEventOccured(
    const util::Identifiable::UUID uuid, const controller::Event& event)
{
   const controller::EventIdExt eventIdExt{uuid, event.id};
   mpark::visit(
       util::overload{
           [this, &eventIdExt](const controller::PressReleaseType& value) {
              handlePressReleaseType(eventIdExt, value);
           },
           [this, &eventIdExt](const controller::ContinousValueType& value) {
              handleContinousValueType(eventIdExt, value);
           },
           [this, &eventIdExt](const controller::IncrementType& value) {
              handleIncrementType(eventIdExt, value);
           },
           [this, &eventIdExt](const controller::RelativeValueType& value) {
              handleRelativeValueType(eventIdExt, value);
           },
           [this](auto&&) {}},
       event.value);
}

namespace detail
{
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
                  const controller::PressReleaseType& value,
                  MDCoords... mdCoords)
{
   if (parameter.descriptionCache.isList)
   {
      if (value.value > 0)
      {
         const float incr =
             m_rParameterCacheMap.upwards ? value.value : -value.value;
         dev.incrementParameterValue(mdCoords..., parameter.id, incr, true);
      }
   }
   else
   {
      if (value.value > 0)
      {
         const float actualVal =
             dev.getParameterValue(mdCoords..., parameter.id);
         if (std::fabs(actualVal - parameter.descriptionCache.zeroVal) <
             std::numeric_limits<float>::epsilon())
         {
            if (parameter.cache.valueAtPress)
            {
               dev.setParameterValue(mdCoords..., parameter.id,
                                     *parameter.cache.valueAtPress);
            }
         }
         else
         {
            parameter.cache.valueAtPress =
                dev.getParameterValue(mdCoords..., parameter.id);
            dev.setParameterValue(mdCoords..., parameter.id,
                                  parameter.descriptionCache.zeroVal);
         }
      }
   }
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::ContinousValueType& value,
                  MDCoords... mdCoords)
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
                  const controller::IncrementType& increment,
                  MDCoords... mdCoords)
{
   float incr = 0;
   if (parameter.descriptionCache.isList)
   {
      const int accIncr = increment.value + parameter.cache.storedIncrements;
      const int incrForOneStep         = increment.resolution / 12;
      incr                             = accIncr / incrForOneStep;
      parameter.cache.storedIncrements = accIncr % incrForOneStep;
   }
   else
   {   // TODO: highres mode
      incr = float(increment.value) / float(increment.resolution);
   }
   dev.incrementParameterValue(mdCoords..., parameter.id, incr, false);
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeValueType& value,
                  MDCoords... mdCoords)
{
   float valueToSet = value.value;
   if (!parameter.cache.valueAtPress)
   {
      parameter.cache.valueAtPress.emplace<float>(
          dev.getParameterValue(mdCoords..., parameter.id));
   }
   valueToSet += parameter.cache.valueAtPress.value();
   dev.setParameterValue(mdCoords..., parameter.id, valueToSet);
   if (0 == value.value)
   {
      parameter.cache.valueAtPress = std::nullopt;
   }
}
}   // namespace detail

void EventRouterRt::handlePressReleaseType(
    const controller::EventIdExt& eventIdExt,
    const controller::PressReleaseType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt,
            &value](const controller::WidgetCoord& widgetCoord) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handlePressRelease(destIter->second, value);
              }
              else
              {
                 controller::EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord
                     .emplace<controller::WidgetCoord>(ANY, ANY);
                 const auto destIter2 = m_rMap.find(melodicEvent);
                 if (destIter2 != m_rMap.end())
                 {
                    handleAnyWidgetCoordPressRelease(widgetCoord,
                                                     destIter2->second, value);
                 }
              }
           },
           [this, &eventIdExt, &value](const controller::Note& note) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handlePressRelease(destIter->second, value);
              }
              else
              {
                 controller::EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<controller::Note>(
                     ANY);
                 const auto destIter2 = m_rMap.find(melodicEvent);
                 if (destIter2 != m_rMap.end())
                 {
                    handleAnyNotePressRelease(note.number, destIter2->second,
                                              value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouterRt::handleContinousValueType(
    const controller::EventIdExt& eventIdExt,
    const controller::ContinousValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt,
            &value](const controller::WidgetCoord& widgetCoord) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
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
           [this, &eventIdExt, &value](const controller::Note& note) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handleContinousValue(destIter->second, value);
              }
              else
              {
                 controller::EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<controller::Note>(
                     ANY);
                 const auto destIter2 = m_rMap.find(melodicEvent);
                 if (destIter2 != m_rMap.end())
                 {
                    sendMPEContinousValue(note.number, destIter2->second,
                                          value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouterRt::handleIncrementType(
    const controller::EventIdExt& eventIdExt,
    const controller::IncrementType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt,
            &value](const controller::WidgetCoord& widgetCoord) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handleIncrement(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const controller::Note& note) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handleIncrement(destIter->second, value);
              }
              else
              {
                 controller::EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<controller::Note>(
                     ANY);
                 const auto destIter2 = m_rMap.find(melodicEvent);
                 if (destIter2 != m_rMap.end())
                 {
                    sendMPEIncrementValue(note.number, destIter2->second,
                                          value);
                 }
              }
           },
           [this](auto&&) { assert(false); }},
       eventIdExt.eventId.widgetCoord);
}

void EventRouterRt::handleRelativeValueType(
    const controller::EventIdExt& eventIdExt,
    const controller::RelativeValueType& value) noexcept
{
   mpark::visit(
       util::overload{
           [this, &eventIdExt,
            &value](const controller::WidgetCoord& widgetCoord) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handleRelativeValue(destIter->second, value);
              }
           },
           [this, &eventIdExt, &value](const controller::Note& note) {
              const auto destIter = m_rMap.find(eventIdExt);
              if (destIter != m_rMap.end())
              {
                 handleRelativeValue(destIter->second, value);
              }
              else
              {
                 controller::EventIdExt melodicEvent = eventIdExt;
                 melodicEvent.eventId.widgetCoord.emplace<controller::Note>(
                     ANY);
                 const auto destIter2 = m_rMap.find(melodicEvent);
                 if (destIter2 != m_rMap.end())
                 {
                    sendMPERelativeValue(note.number, destIter2->second, value);
                 }
              }
           },
           [this](auto&&) {}},
       eventIdExt.eventId.widgetCoord);
}

void EventRouterRt::playNoteOnDrumKit(
    const EventDestination::DrumKit& drumKit,
    const EventDestination::Note& note,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      detail::playNoteOnOff(kitInstr, note.pitch, value.value,
                            drumKit.voiceIdx);
   });
}

void EventRouterRt::setParameterOnDrumKit(
    const EventDestination::DrumKit& drumKit,
    const EventDestination::Parameter& parameter,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      detail::setParameter(kitInstr, parameter, value, drumKit.voiceIdx,
                           drumKit.componentIdx);
   });
}

void EventRouterRt::setParameterOnMelodic(
    const EventDestination::Melodic& melodic,
    const EventDestination::Parameter& parameter,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withMelodicInstrument(melodic.uuid, [&](auto& melodicInstr) {
      detail::setParameter(melodicInstr, parameter, value,
                           melodic.componentIdx);
   });
}

void EventRouterRt::playNoteOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Note& note,
    const controller::PressReleaseType& value) noexcept
{
   m_rMusicDeviceContainer.withSoundHandler(
       musicDevice.uuid, [&](auto& soundHandler) {
          detail::playNoteOnOff(soundHandler, note.pitch, value.value,
                                musicDevice.voiceIdx);
       });
}

void EventRouterRt::setParameterOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Parameter& parameter,
    const controller::PressReleaseType& value) noexcept

{
   m_rMusicDeviceContainer.withSoundHandler(
       musicDevice.uuid, [&](auto& soundHandler) {
          detail::setParameter(soundHandler, parameter, value,
                               musicDevice.voiceIdx);
       });
}

void EventRouterRt::handlePressRelease(
    const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
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

void EventRouterRt::playLayoutMappedDrumKit(
    const controller::WidgetCoord& widgetCoord,
    EventDestination::DrumKit& drumKit,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrument(drumKit.uuid, [&](auto& kitInstr) {
      detail::playNoteOnOff(kitInstr, 64, value.value,
                            widgetCoord.row * 8 + widgetCoord.col);
   });
}

void EventRouterRt::handleAnyWidgetCoordPressRelease(
    const controller::WidgetCoord& widgetCoord,
    const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
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

void EventRouterRt::handleAnyNotePressRelease(
    int note, const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
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
                                   detail::playNoteOnOff(kitInstr, note,
                                                         value.value);
                                });
                         }
                         else
                         {
                            m_rInstruments.withKitInstrument(
                                drumKit.uuid, [&](auto& kitInstr) {
                                   detail::playNoteOnOff(kitInstr, note,
                                                         value.value,
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
                                detail::playNoteOnOff(melodicInstr, note,
                                                      value.value);
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
                                detail::playNoteOnOff(soundaHandler, note,
                                                      value.value,
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

void EventRouterRt::handleContinousValue(
    const EventDestination& eventDestination,
    const controller::ContinousValueType& value) noexcept
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
                                detail::setParameter(kitInstr, parameter, value,
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
                                detail::setParameter(melodicInstr, parameter,
                                                     value,
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
                                detail::setParameter(soundHandler, parameter,
                                                     value,
                                                     musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouterRt::sendMPEContinousValue(
    int note, const EventDestination& eventDestination,
    const controller::ContinousValueType& value) noexcept
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
                                detail::setParameter(melodicInstr, parameter,
                                                     value, note,
                                                     melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](EventDestination::MusicDevice&) {}, [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouterRt::handleIncrement(
    const EventDestination& eventDestination,
    const controller::IncrementType& increment) noexcept
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
                                detail::setParameter(
                                    kitInstr, parameter, increment,
                                    drumKit.voiceIdx, drumKit.componentIdx);
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
                                detail::setParameter(melodicInstr, parameter,
                                                     increment,
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
                                detail::setParameter(soundHandler, parameter,
                                                     increment,
                                                     musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouterRt::sendMPEIncrementValue(
    int note, const EventDestination& eventDestination,
    const controller::IncrementType& increment) noexcept
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
                                detail::setParameter(melodicInstr, parameter,
                                                     increment, note,
                                                     melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [&, this](EventDestination::MusicDevice& musicDevice) {},
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouterRt::handleRelativeValue(
    const EventDestination& eventDestination,
    const controller::RelativeValueType& value) noexcept
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
                                detail::setParameter(kitInstr, parameter, value,
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
                                detail::setParameter(melodicInstr, parameter,
                                                     value,
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
                                detail::setParameter(soundHandler, parameter,
                                                     value,
                                                     musicDevice.voiceIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](auto&&) {}},
       eventDestination.endpoint);
}

void EventRouterRt::sendMPERelativeValue(
    int note, const EventDestination& eventDestination,
    const controller::RelativeValueType& value) noexcept
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
                                detail::setParameter(melodicInstr, parameter,
                                                     value, note,
                                                     melodic.componentIdx);
                             });
                      },
                      [](auto&&) {}},
                  eventDestination.controlType);
           },
           [](EventDestination::MusicDevice&) {}, [](auto&&) {}},
       eventDestination.endpoint);
}