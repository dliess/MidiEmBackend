#include <cstdint>
#include <mpark/variant.hpp>

#include "ControllerColor.h"
#include "ControllerEvents.h"
#include "ControllerSection.h"
#include "MidiMessageIds.h"
#include "pluginIf.h"

std::optional<std::vector<midi::MidiMessage>> createEnlightLedMidiMsg(
   const base::musicDevice::description::controller::Widget& widgetDescr,
   const base::musicDevice::controller::WidgetCoord& widgetCoord,
   const base::musicDevice::controller::ColorARGB& colorARGB)
{
   using namespace base::musicDevice;
   const auto midiMsgId = R_SWITCH(widgetDescr.events[0])
      FCASE(description::controller::EventPressRelease, evt) -> midi::MidiMessageId {
            return evt.pressSource[widgetCoord.row][widgetCoord.col];
      },
      FCASE(description::controller::EventContinousValue, evt) -> midi::MidiMessageId {
            return evt.source[widgetCoord.row][widgetCoord.col];
      },
      FCASE(description::controller::EventRelativeValue, evt) -> midi::MidiMessageId {
            return evt.source[widgetCoord.row][widgetCoord.col];
      },
      FCASE(description::controller::EventDerivedRelativeValue, evt) -> midi::MidiMessageId {
            return mpark::monostate();
      },
      FCASE(description::controller::EventIncremental, evt) -> midi::MidiMessageId {
            return evt.source[widgetCoord.row][widgetCoord.col];
      }
   R_END_SWITCH

   uint8_t color{0};
   static constexpr int RED_0      = 0;
   static constexpr int RED_1      = 1;
   static constexpr int COPY_FLAG  = 2;
   static constexpr int CLEAR_FLAG = 3;
   static constexpr int GREEN_0    = 4;
   static constexpr int GREEN_1    = 5;
   color |= (((colorARGB.r >> 7) & 0x01) << RED_0);
   color |= (((colorARGB.r >> 6) & 0x01) << RED_1);
   color |= (1 << COPY_FLAG);  // No double buffering use
   color |= (1 << CLEAR_FLAG); // No double buffering use
   color |= (((colorARGB.g >> 7) & 0x01) << GREEN_0);
   color |= (((colorARGB.g >> 6) & 0x01) << GREEN_1);

   midi::MidiMessage midiMsg = mpark::visit(
      midi::overload{
         [color](const midi::MidiMsgId<midi::NoteOn>& msgId) -> midi::MidiMessage {
            return midi::Message<midi::NoteOn>(0, msgId.note, color);
         },
         [color](const midi::MidiMsgId<midi::NoteOff>& msgId) -> midi::MidiMessage {
            return midi::Message<midi::NoteOff>(0, msgId.note, color);
         },
         [color](
            const midi::MidiMsgId<midi::ControlChange>& msgId) -> midi::MidiMessage {
            return midi::Message<midi::ControlChange>(0, msgId.id, color);
         },
         [](auto &&) -> midi::MidiMessage { return mpark::monostate(); }},
      midiMsgId);

   return std::vector<midi::MidiMessage>{midiMsg};
}