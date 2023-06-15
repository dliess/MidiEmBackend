#include "ControllerEventRouterRt.h"

#include "ControllerHandler.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"

using namespace base;
using namespace base::eventRouter;
using namespace base::musicDevice;

EventRouterRt::EventRouterRt(const Data& rMap,
                             instruments::InstrumentsRef rInstruments,
                             MusicDeviceContainerRef rMusicDeviceContainer) :
    m_rMap(rMap),
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}
//clang-format off
void EventRouterRt::operator()(const util::Identifiable::UUID uuid,
                               const controller::Event& event)
{
   const controller::EventIdExt eventIdExt{uuid, event.id};
   SWITCH(event.value)
      CASE_MONOSTATE {},
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
      [this, &eventIdExt](const controller::RelativeUnlimitedValueType& value) {
         handleRelativeUnlimitedValueType(eventIdExt, value);
      }
   END_SWITCH
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

bool isList(const EventDestination::Parameter& parameter) {
   return (parameter.parameterAttr == ParameterAttr::Commanded && parameter.descriptionCache.isList) || 
          parameter.parameterAttr == ParameterAttr::LfoMultiplierExp ||
          parameter.parameterAttr == ParameterAttr::LfoWaveform;
}

bool isPress(const controller::PressReleaseType& value) {
   return value.value > 0;
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::PressReleaseType& value,
                  MDCoords... mdCoords)
{
   if (!isPress(value)) return;
   if (isList(parameter))
   {
      const float incr =
            parameter.descriptionCache.upwards ? value.value : -value.value;
      dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr, true);
   }
   else
   {
      const auto actualVal =
            dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      if(!actualVal) {
         return;
      }
      if (std::fabs(actualVal.value() - parameter.descriptionCache.zeroVal) <
            std::numeric_limits<float>::epsilon())
      {
         if (parameter.valueCache->valueAtPress)
         {
            dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr,
                                    *parameter.valueCache->valueAtPress);
         }
      }
      else
      {
         parameter.valueCache->valueAtPress = actualVal;
         dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr,
                                 parameter.descriptionCache.zeroVal);
      }
   }
}

inline
bool isNearEnough(float newVal, float actualVal)
{
   const float diff = std::fabs(newVal - actualVal);
   return ((diff != 0) && (diff < 0.12 || diff >= 1.0));
}

inline int calcIncrements(const EventDestination::Parameter& parameter, 
                          const controller::IncrementType& increment)
{
   if (isList(parameter))
   {
      const int accIncr = increment.value + parameter.valueCache->storedIncrements;
      const int incrForOneStep        = increment.resolution / 12;
      parameter.valueCache->storedIncrements = accIncr % incrForOneStep;
      return accIncr / incrForOneStep;
   }
   else
   {   // TODO: highres mode
      return float(increment.value) / float(increment.resolution);
   }
}


template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::ContinousValueType& value,
                  MDCoords... mdCoords)
{
   const float val = dev.normalizePercentageValue(mdCoords..., parameter.id, base::musicDevice::sound::ParameterAttr::Commanded, value.value);
   const auto actualVal = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
   if(actualVal && isNearEnough(actualVal.value(), val))
   {
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, val);
   }
}
void setParameterMPE(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                     const controller::ContinousValueType& value, int note, int componentIdx)
{
   const float val = dev.normalizePercentageValue(note, componentIdx, parameter.id, base::musicDevice::sound::ParameterAttr::Commanded, value.value);
   const auto actualVal = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
   if(actualVal && isNearEnough(actualVal.value(), val))
   {
      dev.setParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr, val);
   }
}


template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::IncrementType& increment,
                  MDCoords... mdCoords)
{
   const int incr = calcIncrements(parameter, increment);
   dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr, false);
}


template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeValueType& value,
                  MDCoords... mdCoords)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, valueToSet);
   }
}

void setParameterMPE(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                     const controller::RelativeValueType& value, int note,
                     int componentIdx)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr, valueToSet);
   }
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeUnlimitedValueType& value,
                  MDCoords... mdCoords)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, valueToSet);
   }
}
void setParameterMPE(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeUnlimitedValueType& value, int note,
                  int componentIdx)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr, valueToSet);
   }
}


}   // namespace detail

void EventRouterRt::handlePressReleaseType(
    const controller::EventIdExt& eventIdExt,
    const controller::PressReleaseType& value) noexcept
{
   SWITCH(eventIdExt.eventId.widgetCoord)
      CASE_MONOSTATE { assert(false); },
      CASE(controller::WidgetCoord, widgetCoord)
      {
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
      CASE(controller::Note, note)
      {
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
            else
            {
               spdlog::error("Should have found\n {}\n in \n{}", 
                  nlohmann::json(melodicEvent).dump(3),
                  nlohmann::json(m_rMap).dump(3));
            }
         }
      }
   END_SWITCH
}

void EventRouterRt::handleContinousValueType(
    const controller::EventIdExt& eventIdExt,
    const controller::ContinousValueType& value) noexcept
{
   SWITCH(eventIdExt.eventId.widgetCoord)
      CASE_MONOSTATE 
      { 
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleContinousValue(destIter->second, value);
         }
      },
      CASE(controller::WidgetCoord, _)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleContinousValue(destIter->second, value);
         }
         /*
         else
         {
            spdlog::info("Not match found for event: {}\n in map:\n",
                        nlohmann::json(eventIdExt).dump().c_str());
            printMap();
         }
         */
      },
      CASE(controller::Note, note)
      {
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
      }
   END_SWITCH
}

void EventRouterRt::handleIncrementType(
    const controller::EventIdExt& eventIdExt,
    const controller::IncrementType& value) noexcept
{
   SWITCH(eventIdExt.eventId.widgetCoord)
      CASE_MONOSTATE 
      { 
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleIncrement(destIter->second, value);
         }
      },
      CASE(controller::WidgetCoord, _)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleIncrement(destIter->second, value);
         }
      },
      CASE(controller::Note, note)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleIncrement(destIter->second, value);
         }
      }
   END_SWITCH
}

void EventRouterRt::handleRelativeValueType(
    const controller::EventIdExt& eventIdExt,
    const controller::RelativeValueType& value) noexcept
{
   SWITCH(eventIdExt.eventId.widgetCoord)

      CASE_MONOSTATE
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleRelativeValue(destIter->second, value);
         }
      },
      CASE(controller::WidgetCoord, _)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleRelativeValue(destIter->second, value);
         }
      },
      CASE(controller::Note, note)
      {
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
      [this](auto&&) {}
   END_SWITCH
}

void EventRouterRt::handleRelativeUnlimitedValueType(
    const controller::EventIdExt& eventIdExt,
    const controller::RelativeUnlimitedValueType& value) noexcept
{
   SWITCH(eventIdExt.eventId.widgetCoord)

      CASE_MONOSTATE
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleRelativeUnlimitedValue(destIter->second, value);
         }
      },
      CASE(controller::WidgetCoord, _)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleRelativeUnlimitedValue(destIter->second, value);
         }
      },
      CASE(controller::Note, note)
      {
         const auto destIter = m_rMap.find(eventIdExt);
         if (destIter != m_rMap.end())
         {
            handleRelativeUnlimitedValue(destIter->second, value);
         }
         else
         {
            controller::EventIdExt melodicEvent = eventIdExt;
            melodicEvent.eventId.widgetCoord.emplace<controller::Note>(
               ANY);
            const auto destIter2 = m_rMap.find(melodicEvent);
            if (destIter2 != m_rMap.end())
            {
               sendMPERelativeUnlimitedValue(note.number, destIter2->second, value);
            }
         }
      },
      [this](auto&&) {}
   END_SWITCH
}

void EventRouterRt::playNoteOnDrumKit(
    const EventDestination::DrumKit& drumKit,
    const EventDestination::Note& note,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrumentRt(drumKit.uuid, [&](auto& kitInstr) {
      detail::playNoteOnOff(kitInstr, note.pitch, value.value,
                            drumKit.voiceIdx);
   });
}

void EventRouterRt::setParameterOnDrumKit(
    const EventDestination::DrumKit& drumKit,
    const EventDestination::Parameter& parameter,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrumentRt(drumKit.uuid, [&](auto& kitInstr) {
      detail::setParameter(kitInstr, parameter, value,
                           drumKit.voiceIdx, drumKit.componentIdx);
   });
}

void EventRouterRt::setParameterOnMelodic(
    const EventDestination::Melodic& melodic,
    const EventDestination::Parameter& parameter,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withMelodicInstrumentRt(
       melodic.uuid, [&](auto& melodicInstr) {
          detail::setParameter(melodicInstr, parameter,
                               value, melodic.componentIdx);
       });
}

void EventRouterRt::playNoteOnMusicDevice(
    const EventDestination::MusicDevice& musicDevice,
    const EventDestination::Note& note,
    const controller::PressReleaseType& value) noexcept
{
   m_rMusicDeviceContainer.withSoundHandler(
       musicDevice.mdid, [&](auto& soundHandler) {
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
       musicDevice.mdid, [&](auto& soundHandler) {
          detail::setParameter(soundHandler, parameter, 
                               value, musicDevice.voiceIdx);
       });
}

//clang-format off
void EventRouterRt::handlePressRelease(
    const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE_1(drumKit) 
      {
         SWITCH(eventDestination.controlType)
            CASE_1(note) { playNoteOnDrumKit(drumKit, note, value); },
            CASE_2(parameter) { setParameterOnDrumKit(drumKit, parameter, value); }
         END_SWITCH
      },
      CASE_2(melodic) 
      {
         SWITCH(eventDestination.controlType)
            CASE_1(note) { /*EMPTY*/ },
            CASE_2(parameter) { setParameterOnMelodic(melodic, parameter, value); }
         END_SWITCH
      },
      CASE_3(musicDevice) 
      {
         SWITCH(eventDestination.controlType)
            CASE_1(note) { playNoteOnMusicDevice(musicDevice, note, value); },
            CASE_2(parameter) { setParameterOnMusicDevice(musicDevice, parameter, value); }
         END_SWITCH
      }
   END_SWITCH
}
//clang-format on

void EventRouterRt::playLayoutMappedDrumKit(
    const controller::WidgetCoord& widgetCoord,
    const EventDestination::DrumKit& drumKit,
    const controller::PressReleaseType& value) noexcept
{
   m_rInstruments.withKitInstrumentRt(drumKit.uuid, [&](auto& kitInstr) {
      detail::playNoteOnOff(kitInstr, 64, value.value,
                            widgetCoord.row * 8 + widgetCoord.col);
   });
}

void EventRouterRt::handleAnyWidgetCoordPressRelease(
    const controller::WidgetCoord& widgetCoord,
    const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE_1(drumKit) {
         SWITCH(eventDestination.controlType)
            CASE_1(note) { playLayoutMappedDrumKit(widgetCoord, drumKit, value); },
            CASE_2(parameter) {}
         END_SWITCH
      },
      CASE_DEFAULT {}
   END_SWITCH
}

void EventRouterRt::handleAnyNotePressRelease(
    int note, const EventDestination& eventDestination,
    const controller::PressReleaseType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit, drumKit)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note, dstNote)
            {
               if (drumKit.voiceIdx == ANY)
               {
                  m_rInstruments.withKitInstrumentRt(
                     drumKit.uuid, [&](auto& kitInstr) {
                        detail::playNoteOnOff(kitInstr, note,
                                             value.value);
                     });
               }
               else
               {
                  m_rInstruments.withKitInstrumentRt(
                     drumKit.uuid, [&](auto& kitInstr) {
                        detail::playNoteOnOff(kitInstr, note,
                                             value.value,
                                             drumKit.voiceIdx);
                     });
               }
            },
            CASE(EventDestination::Parameter, parameter) {}
         END_SWITCH
      },
      CASE(EventDestination::Melodic, melodic)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note, dstNote)
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::playNoteOnOff(melodicInstr, note,
                                          value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {}
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice, musicDevice)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note, dstNote)
            {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     detail::playNoteOnOff(soundHandler, note,
                                          value.value,
                                          musicDevice.voiceIdx);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {}
         END_SWITCH
      }
   END_SWITCH
}

void EventRouterRt::handleContinousValue(
    const EventDestination& eventDestination,
    const controller::ContinousValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit, drumKit)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter)
            {
               m_rInstruments.withKitInstrumentRt(
                  drumKit.uuid, [&](auto& kitInstr) {
                     detail::setParameter(kitInstr, parameter, value,
                                          drumKit.voiceIdx,
                                          drumKit.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::Melodic, melodic)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter)
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameter(melodicInstr, parameter,
                                          value,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice, musicDevice)
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note, _) {},
            CASE(EventDestination::Parameter, parameter)
            {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     detail::setParameter(soundHandler, parameter,
                                          value,
                                          musicDevice.voiceIdx);
                  });
            }
         END_SWITCH
      }
   END_SWITCH
}

void EventRouterRt::sendMPEContinousValue(
    int note, const EventDestination& eventDestination,
    const controller::ContinousValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit,_) {},
      CASE(EventDestination::Melodic, melodic) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter) 
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameterMPE(melodicInstr, parameter,
                                          value, note,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice,_) {}
   END_SWITCH
}

void EventRouterRt::handleIncrement(
    const EventDestination& eventDestination,
    const controller::IncrementType& increment) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit, drumKit) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withKitInstrumentRt(
                  drumKit.uuid, [&](auto& kitInstr) {
                     detail::setParameter(
                        kitInstr, parameter, 
                        increment, drumKit.voiceIdx,
                        drumKit.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::Melodic, melodic) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameter(melodicInstr, parameter,
                                          increment,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice, musicDevice) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter) {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     detail::setParameter(soundHandler, parameter,
                                          increment,
                                          musicDevice.voiceIdx);
                  });
            }
         END_SWITCH
      }
   END_SWITCH
}

void EventRouterRt::handleRelativeValue(
    const EventDestination& eventDestination,
    const controller::RelativeValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit, drumKit) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               spdlog::error("for now we dont do pitchbend on drumkit");
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withKitInstrumentRt(
                  drumKit.uuid, [&](auto& kitInstr) {
                     detail::setParameter(kitInstr, parameter,
                                          value, drumKit.voiceIdx,
                                          drumKit.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::Melodic, melodic) {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     melodicInstr.pitchBend(value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameter(melodicInstr, parameter,
                                          value,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice, musicDevice) {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     soundHandler.pitchBend(musicDevice.voiceIdx, value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     detail::setParameter(soundHandler, parameter,
                                          value,
                                          musicDevice.voiceIdx);
                  });
            }
         END_SWITCH
      }
   END_SWITCH
}

void EventRouterRt::handleRelativeUnlimitedValue(
    const EventDestination& eventDestination,
    const controller::RelativeUnlimitedValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit, drumKit) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               spdlog::error("Empty HERE");
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withKitInstrumentRt(
                  drumKit.uuid, [&](auto& kitInstr) {
                     detail::setParameter(kitInstr, parameter,
                                          value, drumKit.voiceIdx,
                                          drumKit.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::Melodic, melodic) {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     melodicInstr.pitchBend(value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameter(melodicInstr, parameter,
                                          value,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice, musicDevice) {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     soundHandler.pitchBend(musicDevice.voiceIdx, value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) {
               m_rMusicDeviceContainer.withSoundHandler(
                  musicDevice.mdid, [&](auto& soundHandler) {
                     detail::setParameter(soundHandler, parameter,
                                          value,
                                          musicDevice.voiceIdx);
                  });
            }
         END_SWITCH
      }
   END_SWITCH
}


void EventRouterRt::sendMPERelativeValue(
    int note, const EventDestination& eventDestination,
    const controller::RelativeValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit,_) {},
      CASE(EventDestination::Melodic, melodic) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) {},
            CASE(EventDestination::Parameter, parameter) 
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameterMPE(melodicInstr, parameter,                                                    
                                          value, note,
                                          melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice,_) {}
   END_SWITCH
}

void EventRouterRt::sendMPERelativeUnlimitedValue(
    int note, const EventDestination& eventDestination,
    const controller::RelativeUnlimitedValueType& value) noexcept
{
   SWITCH(eventDestination.endpoint)
      CASE(EventDestination::DrumKit,_) {
         spdlog::error("sendMPERelativeUnlimitedValue on drumkit, does it make sense?");
      },
      CASE(EventDestination::Melodic, melodic) 
      {
         SWITCH(eventDestination.controlType)
            CASE(EventDestination::Note,_) 
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     melodicInstr.pitchBend(note, value.value);
                  });
            },
            CASE(EventDestination::Parameter, parameter) 
            {
               m_rInstruments.withMelodicInstrumentRt(
                  melodic.uuid, [&](auto& melodicInstr) {
                     detail::setParameterMPE(melodicInstr, parameter,                                                    
                                             value, note,
                                             melodic.componentIdx);
                  });
            }
         END_SWITCH
      },
      CASE(EventDestination::MusicDevice,_) {}
   END_SWITCH
}