#include <gtest/gtest.h>

#include "MidiDumper.h"

using namespace base::midifriends;

TEST(MidiDumperTest, test)
{
   base::musicDevice::MidiHolder midiHolder;
   Dumper dumper(midiHolder);
}