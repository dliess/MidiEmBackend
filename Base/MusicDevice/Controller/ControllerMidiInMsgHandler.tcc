#include "ControllerMidiInMsgHandler.h"
#include "ControllerSection.h"
#include "MidiMessage.h"
#include <loguru.hpp>

namespace base::musicDevice
{

template<typename MidiInIfPtr>
std::string controller::MidiInMsgHandler<MidiInIfPtr>::cache2Str(
   const std::unordered_map<midi::MidiMessageId, EventId>& map)
{
   std::string ret;
   for (auto iter = map.begin(); iter != map.end(); iter++)
   {
      ret.append(meta::serialize(iter->first).dump() + " --> " +
                 meta::serialize(iter->second).dump() + "\n");
   }
   return ret;
}

template<typename MidiInIfPtr>
controller::MidiInMsgHandler<MidiInIfPtr>::MidiInMsgHandler(
      MidiInIfPtr pMidiInIf,
      const description::controller::Section& rControllerSection,
      Cb cb) noexcept:
   m_pMidiInIf(std::move(pMidiInIf)),
   m_rControllerSection(rControllerSection),
   m_drainCb(cb)
{
   initCache();
   m_pMidiInIf->registerMidiInCb([this](const midi::MidiMessage& midiMsg) {
      const auto midiId = midiMessageToId(midiMsg);
      auto iter         = m_map.find(midiId);
      if (m_map.end() == iter)
      {
         LOG_F(ERROR, "No mapping for Midi msg id {}",
               meta::serialize(midiId).dump());
         return;
      }
      handleRouting(iter->second, midiMsg);
   });
}


template<typename MidiInIfPtr>
void controller::MidiInMsgHandler<MidiInIfPtr>::enableNativeNoteMode(bool enable) noexcept
{
   m_nativeNoteMode = enable;
}

template<typename MidiInIfPtr>
void controller::MidiInMsgHandler<MidiInIfPtr>::handleRouting(
   EventId id, const midi::MidiMessage& midiMsg) const noexcept
{
   const auto& eventDescr =
      m_rControllerSection.widgets[id.widgetId].events[id.eventId];

   const bool mpeMode = (m_rControllerSection.widgets[id.widgetId].mpe &&
                         m_rControllerSection.widgets[id.widgetId].mpe);


   const auto value = mpark::visit(
      midi::overload{
         [this, &id, &eventDescr](const midi::Message<midi::ControlChange>& msg)
            -> EventValue {
            if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
            {
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventPressRelease& evt)
                     -> EventValue {
                        if(evt.sourceHasInvertedLogic && *evt.sourceHasInvertedLogic){
                           return PressReleaseType{
                              msg.controllerValue() ? -1.0f : 1.0f};
                        }
                        else{
                           return PressReleaseType{
                              msg.controllerValue() ? 1.0f : -1.0f};
                        }
                  },
                  [this, &msg](const description::controller::EventContinousValue& evt)
                     -> EventValue {
                     return ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const description::controller::EventRelativeValue& evt)
                     -> EventValue {
                     return RelativeValueType{msg.getRelativeValue() - 0.5f};
                  },
                  [this, &msg](const description::controller::EventIncremental& evt)
                     -> EventValue {
                     return IncrementType{
                        msg.controllerValue() -
                        (midi::Message<midi::ControlChange>::RES_MAX / 2)};
                  }},
               eventDescr);
         },
         [this, &id,
          &eventDescr](const midi::Message<midi::ControlChangeHighRes>& msg)
            -> EventValue {
            if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
            {
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventPressRelease& evt)
                     -> EventValue {
                     return PressReleaseType{
                        msg.controllerValue() ? 1.0f : -1.0f};
                  },
                  [this, &msg](const description::controller::EventContinousValue& evt)
                     -> EventValue {
                     return ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const description::controller::EventRelativeValue& evt)
                     -> EventValue {
                     return RelativeValueType{msg.getRelativeValue() - 0.5f};
                  },
                  [this, &msg](const description::controller::EventIncremental& evt)
                     -> EventValue {
                     return IncrementType{
                        msg.controllerValue() -
                        (midi::Message<midi::ControlChangeHighRes>::RES_MAX /
                         2)};
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr](
            const midi::Message<midi::NRPN>& msg) -> EventValue {
            if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
            {
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventPressRelease& evt)
                     -> EventValue {
                     return PressReleaseType{msg.getValue() ? 1.0f
                                                                     : -1.0f};
                  },
                  [this, &msg](const description::controller::EventContinousValue& evt)
                     -> EventValue {
                     return ContinousValueType{msg.getRelativeValue()};
                  },
                  [this, &msg](const description::controller::EventRelativeValue& evt)
                     -> EventValue {
                     return RelativeValueType{msg.getRelativeValue() - 0.5f};
                  },
                  [this, &msg](const description::controller::EventIncremental& evt)
                     -> EventValue {
                     return IncrementType{
                        msg.getValue() -
                        (midi::Message<midi::NRPN>::RES_MAX / 2)};
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr, mpeMode](
            const midi::Message<midi::NoteOn>& msg) -> EventValue {
            if (m_nativeNoteMode)
            {
               id.widgetCoord = Note{msg.noteNumber()};
            }
            if (mpeMode)
            {
               if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
               {
                  LOG_F(ERROR, "MPE mode note-on without widget coordinate");
                  return mpark::monostate();
               }
               m_mpeMap[msg.channel() - 1] = id.widgetCoord;
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventPressRelease& evt)
                     -> EventValue {
                     return PressReleaseType{msg.relativeVelocity()};
                  },
                  [](auto &&) -> EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr](
            const midi::Message<midi::NoteOff>& msg) -> EventValue {
            if (m_nativeNoteMode)
            {
               id.widgetCoord = Note{msg.noteNumber()};
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventPressRelease& evt)
                     -> EventValue {
                     return PressReleaseType{-1.0f *
                                                      msg.relativeVelocity()};
                  },
                  [](auto &&) -> EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &id,
          &eventDescr](const midi::Message<midi::AfterTouchChannel>& msg)
            -> EventValue {
            if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
            {
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventContinousValue& evt)
                     -> EventValue {
                     return ContinousValueType{msg.relativeValue()};
                  },
                  [](auto &&) -> EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &eventDescr](const midi::Message<midi::AfterTouchPoly>& msg)
            -> EventValue {
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventContinousValue& evt)
                     -> EventValue {
                     return ContinousValueType{msg.relativePressure()};
                  },
                  [](auto &&) -> EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [this, &id, &eventDescr](
            const midi::Message<midi::PitchBend>& msg) -> EventValue {
            if (mpark::holds_alternative<mpark::monostate>(id.widgetCoord))
            {
               id.widgetCoord = m_mpeMap[msg.channel() - 1];
            }
            return mpark::visit(
               midi::overload{
                  [this, &msg](const description::controller::EventRelativeValue& evt)
                     -> EventValue {
                     return RelativeValueType{
                        float(msg.value())}; // TODO: need some coefficient?
                  },
                  [](auto &&) -> EventValue {
                     return mpark::monostate();
                  }},
               eventDescr);
         },
         [](auto&& other) -> EventValue {
            return mpark::monostate();
         }
      }, midiMsg);
   if (!mpark::holds_alternative<mpark::monostate>(value))
   {
      m_drainCb(Event{id, value});
   }
}

template<typename MidiInIfPtr>
void controller::MidiInMsgHandler<MidiInIfPtr>::handleEventSource(
   const std::vector<std::vector<midi::MidiMessageId>>& source, int widgetId,
   int eventId) noexcept
{
   assert(source.size() > 0 && source[0].size() > 0);
   if (source.size() > 1 || source[0].size() > 1)
   {
      for (int row = 0; row < source.size(); ++row)
      {
         for (int col = 0; col < source[row].size(); ++col)
         {
            m_map[source[row][col]] = EventId{
               widgetId, WidgetCoord{row, col},
               eventId};
         }
      }
   }
   else
   {
      m_map[source[0][0]] = EventId{widgetId, mpark::monostate(), eventId};
   }
}

template<typename MidiInIfPtr>
void controller::MidiInMsgHandler<MidiInIfPtr>::initCache() noexcept
{
   for (int widgetId = 0;
        widgetId < m_rControllerSection.widgets.size(); ++widgetId)
   {
      const auto& widget = m_rControllerSection.widgets[widgetId];
      for (int eventId = 0; eventId < widget.events.size(); ++eventId)
      {
         const auto& event = widget.events[eventId];
         mpark::visit(
            midi::overload{
               [this, widgetId,
                eventId](const description::controller::EventPressRelease& evt) {
                  handleEventSource(evt.pressSource, widgetId, eventId);
                  handleEventSource(evt.releaseSource, widgetId, eventId);
               },
               [this, widgetId,
                eventId](const description::controller::EventContinousValue& evt) {
                  handleEventSource(evt.source, widgetId, eventId);
               },
               [this, widgetId,
                eventId](const description::controller::EventRelativeValue& evt) {
                  handleEventSource(evt.source, widgetId, eventId);
               },
               [this, widgetId,
                eventId](const description::controller::EventIncremental& evt) {
                  handleEventSource(evt.source, widgetId, eventId);
               }},
            event);
      }
   }
}

} // namespace base::musicDevice