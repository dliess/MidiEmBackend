#include <gtest/gtest.h>

#include "ControllerMidiInMsgHandler.h"
#include "ControllerSection.h"
#include "MidiMessage.h"

using namespace base::musicDevice;

class MidiInIfMock
{
public:
   using Cb = std::function<void(const midi::MidiMessage& midiMsg)>;
   void registerMidiInCb(Cb cb) noexcept { m_cb = cb; }
   void setCCHighResPair(int idMsb, int idLsb) noexcept {};

private:
   Cb m_cb;
};

using MidiInMsgHandlerSpec = controller::MidiInMsgHandler<MidiInIfMock*>;

TEST(ControllerMidiInMsgHandlerTest, test)
{
   MidiInIfMock midiInIfMock;
   description::controller::Section controllerSection;
   MidiInMsgHandlerSpec msgHandler(&midiInIfMock, controllerSection,
                                   [](const controller::Event& event) {

                                   });
}