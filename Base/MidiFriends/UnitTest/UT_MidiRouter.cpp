#include <gtest/gtest.h>

#include "MidiRouter.h"
#include "MidiHolder.h"

using namespace base::midifriends;

TEST(MidiRouterTest, test)
{
   base::musicDevice::MidiHolder midiHolder;
   Router router(midiHolder);
}