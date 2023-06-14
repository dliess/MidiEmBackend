//clang-format off
#include <spdlog/spdlog.h>

#include "ControllerMidiInMsgHandler.h"
#include "ControllerSection.h"
#include "MidiMessage.h"

namespace base::musicDevice::controller
{

static constexpr EventId::Coord TO_BE_FILLED_BY_MPE_MARKER = Note{-1};

template <typename MidiInIfPtr>
std::string MidiInMsgHandler<MidiInIfPtr>::cache2Str(
    const std::unordered_map<midi::MidiMessageId, EventId>& map)
{
   std::string ret;
   for (auto iter = map.begin(); iter != map.end(); iter++)
   {
      ret.append(nlohmann::json(iter->first).dump() + " --> " +
                 nlohmann::json(iter->second).dump() + "\n");
   }
   return ret;
}

template <typename MidiInIfPtr>
MidiInMsgHandler<MidiInIfPtr>::MidiInMsgHandler(
    MidiInIfPtr pMidiInIf,
    const description::controller::Section& rControllerSection, Cb cb) noexcept
    :
    m_pMidiInIf(std::move(pMidiInIf)),
    m_rControllerSection(rControllerSection),
    m_drainCb(cb)
{
   initCache();
   m_pMidiInIf->registerMidiInCb([this](const midi::MidiMessage& midiMsg) {
      auto midiId = midiMessageToId(midiMsg);
      auto iter = m_map.find(midiId);
      if (m_map.end() == iter)
      {
         /*
          spdlog::info("CONTROLLER --- {} No mapping for Midi msg id {}",
                       m_pMidiInIf->medium().getDeviceName(),
                       nlohmann::json(midiId).dump());
         */
         return;
      }
      handleRouting(iter->second, midiMsg);
   });
}

inline
std::optional<uint8_t> getMidiChannelFromMsg(const midi::MidiMessage& midiMsg)
{
   return R_SWITCH(midiMsg)
      FFCASE_MONOSTATE -> std::optional<uint8_t> { return std::nullopt; },
      FCASE(midi::Message<midi::NoteOff>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::NoteOn>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::AfterTouchPoly>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::ProgramChange>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::ControlChange>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::AfterTouchChannel>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::PitchBend>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::RPN>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::NRPN>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      FCASE(midi::Message<midi::ControlChangeHighRes>, msg) -> std::optional<uint8_t> { return msg.channel(); },
      CASE_DEFAULT -> std::optional<uint8_t> { return std::nullopt; }
   R_END_SWITCH
}

template <typename MidiInIfPtr>
void MidiInMsgHandler<MidiInIfPtr>::handleRouting(
    EventId id, const midi::MidiMessage& midiMsg) const noexcept
{

   const auto midiChannelNr = getMidiChannelFromMsg(midiMsg);
   if (midiChannelNr && (TO_BE_FILLED_BY_MPE_MARKER == id.widgetCoord))
   {
      id.widgetCoord = m_mpeMap[midiChannelNr.value() - 1];
   }

   const auto& eventDescr =
       m_rControllerSection.widgets[id.widgetId].events[id.eventId];

   const bool mpeMode = m_rControllerSection.widgets[id.widgetId].mpe.value_or(false);
   if(midiChannelNr && !mpeMode)
   {
      id.channelId = midiChannelNr.value() - 1;
   }
   
   const auto value = 
      R_SWITCH(midiMsg)
         CASE(midi::Message<midi::ControlChange>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventPressRelease, evt) -> EventValue 
               {
                  if (evt.sourceHasInvertedLogic.value_or(false))
                  {
                     return PressReleaseType{
                        msg.controllerValue() ? -1.0f : 1.0f};
                  }
                  else
                  {
                     return PressReleaseType{
                        msg.controllerValue() ? 1.0f : -1.0f};
                  }
               },
               CASE(description::controller::EventContinousValue, evt) -> EventValue 
               {
                  return ContinousValueType{msg.getNormalizedValue()};
               },
               CASE(description::controller::EventRelativeValue, evt) -> EventValue 
               {
                  if(evt.bidirectional)
                  {
                     return RelativeValueType{(msg.getNormalizedValue() - 0.5f) * 2.0f};
                  }
                  else
                  {
                     return RelativeValueType{msg.getNormalizedValue()};
                  }
               },
               CASE(description::controller::EventRelativeUnlimitedValue, evt) -> EventValue 
               {
                  return mpark::monostate();
               },
               CASE(description::controller::EventIncremental, evt) -> EventValue 
               {
                  const int ccVal = msg.controllerValue();
                  static constexpr int middleVal =
                     midi::Message<midi::ControlChange>::RES_MAX / 2;
                  static constexpr int THRESHOLD = middleVal / 2;
                  const int diffFromMiddleVal    = ccVal - middleVal;
                  if (std::abs(diffFromMiddleVal) > THRESHOLD)
                  {
                     if (ccVal < middleVal)
                     {
                        return IncrementType{evt.resolution, ccVal};
                     }
                     else
                     {
                        return IncrementType{
                           evt.resolution,
                           ccVal - midi::Message<
                                       midi::ControlChange>::RES_MAX};
                     }
                  }
                  else
                  {
                     return IncrementType{evt.resolution,
                                          diffFromMiddleVal};
                  }
               }
         R_END_SWITCH
         },
         CASE(midi::Message<midi::ControlChangeHighRes>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventPressRelease, evt) -> EventValue 
               {
                  return PressReleaseType{msg.controllerValue() ? 1.0f : -1.0f};
               },
               CASE(description::controller::EventContinousValue, evt) -> EventValue 
               {
                  return ContinousValueType{msg.getNormalizedValue()};
               },
               CASE(description::controller::EventRelativeValue, evt) -> EventValue
               {
                  if(evt.bidirectional)
                  {
                     return RelativeValueType{(msg.getNormalizedValue() - 0.5f) * 2.0f};
                  }
                  else
                  {
                     return RelativeValueType{msg.getNormalizedValue()};
                  }
               },
               CASE(description::controller::EventRelativeUnlimitedValue, evt) -> EventValue 
               {
                  return mpark::monostate();
               },
               CASE(description::controller::EventIncremental, evt) -> EventValue 
               {
                  return IncrementType{
                     msg.controllerValue() - (midi::Message<midi::ControlChangeHighRes>::RES_MAX / 2)};
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::NRPN>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventPressRelease, evt) -> EventValue 
               {
                  return PressReleaseType{msg.getValue() ? 1.0f : -1.0f};
               },
               CASE(description::controller::EventContinousValue, evt) -> EventValue 
               {
                  return ContinousValueType{msg.getNormalizedValue()};
               },
               CASE(description::controller::EventRelativeValue, evt) -> EventValue 
               {
                  if(evt.bidirectional)
                  {
                     return RelativeValueType{(msg.getNormalizedValue() - 0.5f) * 2.0f};
                  }
                  else
                  {
                     return RelativeValueType{msg.getNormalizedValue()};
                  }
               },
               CASE(description::controller::EventRelativeUnlimitedValue, evt) -> EventValue 
               {
                  return mpark::monostate();
               },
               CASE(description::controller::EventIncremental, evt) -> EventValue
               {
                  return IncrementType{
                     msg.getValue() - (midi::Message<midi::NRPN>::RES_MAX / 2)};
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::NoteOn>, msg)
         {
            if (mpeMode)
            {
               m_mpeMap[msg.channel() - 1] = id.widgetCoord;
            }
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventPressRelease, evt) -> EventValue
               {
                  return PressReleaseType{msg.normalizedVelocity()};
               },
               CASE_DEFAULT -> EventValue 
               { 
                  return mpark::monostate();
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::NoteOff>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventPressRelease, evt) -> EventValue
               {
                  return PressReleaseType{-1.0f * msg.normalizedVelocity()};
               },
               CASE_DEFAULT -> EventValue
               {
                  return mpark::monostate();
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::AfterTouchChannel>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventRelativeValue, evt) -> EventValue
               {
                  return RelativeValueType{msg.normalizedValue()};
               },
               CASE_DEFAULT -> EventValue
               {
                  spdlog::error("AfterTouchChannel should always be RelativeValue");
                  return mpark::monostate(); 
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::AfterTouchPoly>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventContinousValue, evt) -> EventValue
               {
                  return ContinousValueType{msg.relativePressure()};
               },
               CASE_DEFAULT -> EventValue
               {
                  return mpark::monostate();
               }
            R_END_SWITCH
         },
         CASE(midi::Message<midi::PitchBend>, msg)
         {
            return R_SWITCH(eventDescr)
               CASE(description::controller::EventRelativeUnlimitedValue, evt) -> EventValue
               {
                  return RelativeUnlimitedValueType{msg.value(), evt.incrementsPerCentimeter};
               },
               CASE_DEFAULT -> EventValue 
               {
                  spdlog::error("<midi::PitchBend should result in an event type: EventRelativeUnlimitedValue");
                  return mpark::monostate(); 
               }
            R_END_SWITCH
         },
         CASE_DEFAULT -> EventValue { return mpark::monostate(); }
   R_END_SWITCH
   if (!mpark::holds_alternative<mpark::monostate>(value))
   {
      m_drainCb(Event{id, value});
   }
   
}

namespace detail
{
bool fillForAnyNote(std::unordered_map<midi::MidiMessageId, EventId>& rMap, 
                    const std::vector<std::vector<midi::MidiMessageId>>& source, 
                    int widgetId, int eventId)
{
   static constexpr int ANY_NOTE = -1;
   static constexpr int MIDI_MAX_NOTE = 128;
   static constexpr int ANY_MIDI_CHANNEL = 0;
   bool ret{false};
   SWITCH(source[0][0])
      FCASE(midi::MidiMsgId<midi::NoteOn>, msg)
      {
         if(msg.note == ANY_NOTE)
         {
            for(int note = 0; note < MIDI_MAX_NOTE; ++note)
            {
               rMap[midi::MidiMsgId<midi::NoteOn>(note)] = EventId{widgetId, Note{note}, eventId, ANY_MIDI_CHANNEL};
            }
            ret = true;
         }
      },
      FCASE(midi::MidiMsgId<midi::NoteOff>, msg)
      {
         if(msg.note == ANY_NOTE)
         {
            for(int note = 0; note < MIDI_MAX_NOTE; ++note)
            {
               rMap[midi::MidiMsgId<midi::NoteOff>(note)] = EventId{widgetId, Note{note}, eventId, ANY_MIDI_CHANNEL};
            }
            ret = true;
         }
      },
      FCASE(midi::MidiMsgId<midi::AfterTouchPoly>, msg)
      {
         if(msg.note == ANY_NOTE)
         {
            for(int note = 0; note < MIDI_MAX_NOTE; ++note)
            {
               rMap[midi::MidiMsgId<midi::NoteOn>(note)] = EventId{widgetId, Note{note}, eventId, ANY_MIDI_CHANNEL};
            }
            ret = true;
         }
      },
      CASE_DEFAULT {}
   END_SWITCH
   return ret;
}

void fillByEveryElement(std::unordered_map<midi::MidiMessageId, EventId>& rMap, 
                        const std::vector<std::vector<midi::MidiMessageId>>& source,
                        int widgetId, int eventId)
{
   for (int row = 0; row < source.size(); ++row)
   {
      for (int col = 0; col < source[row].size(); ++col)
      {
         rMap[source[row][col]] =
            EventId{widgetId, WidgetCoord{row, col}, eventId};
      }
   }
}

void fillMapByEventSourcePR(
    std::unordered_map<midi::MidiMessageId, EventId>& rMap,
    const std::vector<std::vector<midi::MidiMessageId>>& source, 
    int widgetId, int eventId, bool global, bool mpe)
{
   assert(source.size() > 0 && source[0].size() > 0);
   if(global)
   {
      spdlog::error("PressRelease controller event should not be marked as global!!!");
   }
   else
   {
      if(!fillForAnyNote(rMap, source, widgetId, eventId))
      {
         fillByEveryElement(rMap, source, widgetId, eventId);
      }
   }
}

void fillMapByEventSourceNonPR(
    std::unordered_map<midi::MidiMessageId, EventId>& rMap,
    const std::vector<std::vector<midi::MidiMessageId>>& source,
    int widgetId, int eventId, bool global, bool mpe)
{
   assert(source.size() > 0 && source[0].size() > 0);
   if(global)
   {
      rMap[source[0][0]] = EventId{widgetId, mpark::monostate(), eventId};
   }
   else
   {
      if(mpe)
      {
         rMap[source[0][0]] = EventId{widgetId, TO_BE_FILLED_BY_MPE_MARKER, eventId};
      }
      else
      {
         if(!fillForAnyNote(rMap, source, widgetId, eventId))
         {
            fillByEveryElement(rMap, source, widgetId, eventId);
         }
      }
   }
}
} // namespace detail
template <typename MidiInIfPtr>
void MidiInMsgHandler<MidiInIfPtr>::initCache()
{
   for (int widgetId = 0; widgetId < m_rControllerSection.widgets.size();
        ++widgetId)
   {
      const auto& widget = m_rControllerSection.widgets[widgetId];
      for (int eventId = 0; eventId < widget.events.size(); ++eventId)
      {
         const auto& event = widget.events[eventId];
         SWITCH(event)
            CASE(description::controller::EventPressRelease, evt)
            {
               detail::fillMapByEventSourcePR(m_map, evt.pressSource, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
               detail::fillMapByEventSourcePR(m_map, evt.releaseSource, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
            },
            CASE(description::controller::EventContinousValue, evt)
            {
               detail::fillMapByEventSourceNonPR(m_map, evt.source, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
            },
            CASE(description::controller::EventRelativeValue, evt) 
            {
              detail::fillMapByEventSourceNonPR(m_map, evt.source, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
            },
            CASE(description::controller::EventRelativeUnlimitedValue, evt) 
            {
              detail::fillMapByEventSourceNonPR(m_map, evt.source, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
            },
            CASE(description::controller::EventIncremental, evt)
            {
              detail::fillMapByEventSourceNonPR(m_map, evt.source, widgetId, eventId, evt.global.value_or(false), widget.mpe.value_or(false));
            }
         END_SWITCH
      }
   }
}
}   // namespace base::musicDevice
//clang-format on