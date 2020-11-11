#include <gtest/gtest.h>
#include "SoundMidiOutMsgHandler.h"
#include "MidiMessage.h"

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

using OutMsgHandler = sound::MidiOutMsgHandler<MidiOutIfMock*>;

TEST(SoundMidiOutMsgHandlerTest, test)
{
   MidiOutIfMock midiOutIfMock;
   description::sound::Section soundSection;
   OutMsgHandler outMsgHandler(&midiOutIfMock, soundSection);
   outMsgHandler.sendParameterDumpRequest();
   outMsgHandler.sendSoundParameter(1, 2, 12.0);
   outMsgHandler.noteOn(1, 3, 4.0);
   outMsgHandler.noteOff(1, 3, 4.0);
   outMsgHandler.pitchBend(1, 14.0);
}