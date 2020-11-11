#include <gtest/gtest.h>

#include <functional>

#include "SoundMidiInMsgHandler.h"

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

using MidiInMsgHandlerSpec = sound::MidiInMsgHandler<MidiInIfMock*>;

TEST(SoundMidiInMsgHandlerTest, test)
{
   MidiInIfMock midiInIfMock;
   description::sound::Section soundSection;
   MidiInMsgHandlerSpec msgHandler(
      &midiInIfMock, soundSection,
      [](int voiceId, int parameterId, float value) {

      });
}
