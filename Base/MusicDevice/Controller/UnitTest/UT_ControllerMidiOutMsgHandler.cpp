#include <gtest/gtest.h>

#include "ControllerMidiOutMsgHandler.h"
#include "ControllerSection.h"
#include "MidiMessage.h"
#include <optional>
#include <vector>

using namespace base::musicDevice;

class MidiOutIfMock
{
public:
   void sysEx(const std::vector<unsigned char>&) noexcept {};
   void controlParameter(int midiChannel, int ccId, int value) noexcept {};
   void noteOn(int midiChannel, int note, int velocity) noexcept {};
   void noteOff(int midiChannel, int note, int velocity) noexcept {};
   void pitchBend(int midiChannel, float value) noexcept {};
   void send(const midi::MidiMessage& midiMsg) noexcept {};
};

class PluginifMock
{
public:
   std::optional<std::vector<midi::MidiMessage>> createEnlightLedMidiMsg(
      const description::controller::Widget& widgetDescr,
      const controller::WidgetCoord& widgetCoord,
      const controller::ColorARGB& colorARGB) noexcept {
         return std::nullopt;
      }
};

using OutMsgHandler = controller::MidiOutMsgHandler<MidiOutIfMock*, PluginifMock>;

TEST(ControllerMidiOutMsgHandlerTest, test)
{
   MidiOutIfMock midiOutIfMock;
   PluginifMock pluginifMock;
   description::controller::Section controllerSection;
   OutMsgHandler outMsgHandler(&midiOutIfMock, pluginifMock, controllerSection);
   outMsgHandler.enlightLed(0, {2, 2}, {1, 1, 1, 1});
   outMsgHandler.setValueFeedback(0, {2, 2}, 3.4);
}